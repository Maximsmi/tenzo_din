import serial
import threading
import time
from configparser import ConfigParser
from typing import Optional, Callable


class ComHandler:
    def __init__(self, config: ConfigParser, print_func: Callable, update_register_func: Callable):
        self.config = config
        self.print_func = print_func
        self.update_register_func = update_register_func
        self.com_port: Optional[serial.Serial] = None
        self.read_thread: Optional[threading.Thread] = None
        self.running = False
        self.register_queue = []

    def connect(self):
        self.print_func(f"<PC> Подключение к устройству - ", end="")
        try:
            self.com_port = serial.Serial(
                self.config['COM']['port'],
                self.config['COM'].getint('baudrate'),
                timeout=1
            )
            self.print_func("OK")

            # Отправляем стартовое сообщение
            start_msg = self.config['Messages']['start_message']
            self._send_message(start_msg)

            # Запускаем поток для чтения
            self.running = True
            self.read_thread = threading.Thread(target=self._read_from_port)
            self.read_thread.daemon = True
            self.read_thread.start()

            # Отправляем команды с задержкой
            self._send_delayed_commands()

            return True
        except Exception as e:
            self.print_func(f"Error\n<PC> Error: {str(e)}")
            return False

    def disconnect(self):
        if self.com_port and self.com_port.is_open:
            self.running = False
            if self.read_thread and self.read_thread.is_alive():
                self.read_thread.join(timeout=1)
            self.com_port.close()
            self.print_func("<PC> Устройство отключено")
            return True
        return False

    def _send_message(self, message: str):
        """Отправка сообщения через COM-порт"""
        if self.com_port and self.com_port.is_open:
            try:
                self.com_port.write(message.encode())
                self.print_func(f"<PC> Отправлено: {message}")
            except Exception as e:
                self.print_func(f"<PC> Ошибка отправки: {str(e)}")

    def _send_delayed_commands(self):
        """Отправка команд с задержкой"""

        def send_commands():
            prefix = self.config['Messages']['prefix_zsc31050']
            cmd_symbol = self.config['Messages']['command_symbol']
            read_symbol = self.config['Messages']['read_register_symbol']

            # Отправляем команду 72
            time.sleep(0.1)
            self._send_message(f"{prefix}{cmd_symbol}72")

            # Отправляем команду 52
            time.sleep(0.1)
            self._send_message(f"{prefix}{cmd_symbol}52")

            # Формируем очередь запросов регистров
            registers = [
                ('cfgcyc', self.config['Registers']['reg_cfgcyc']),
                ('cfgsif', self.config['Registers']['reg_cfgsif']),
                ('cfgapp', self.config['Registers']['reg_cfgapp']),
                ('cfgafe', self.config['Registers']['reg_cfgafe']),
                ('cfgtmp', self.config['Registers']['reg_cfgtmp']),
                ('cfgout', self.config['Registers']['reg_cfgout']),
                ('adjref', self.config['Registers']['reg_adjref'])
            ]

            # Отправляем запросы регистров с задержкой
            for reg_name, reg_addr in registers:
                time.sleep(0.2)
                self.register_queue.append(reg_name)
                self._send_message(f"{prefix}{read_symbol}{reg_addr}")

        threading.Thread(target=send_commands, daemon=True).start()

    def _read_from_port(self):
        """Чтение данных из COM-порта"""
        while self.running and self.com_port and self.com_port.is_open:
            try:
                if self.com_port.in_waiting:
                    line = self.com_port.readline().decode().strip()
                    if line:
                        self.print_func(f"<Device> {line}")
                        self._process_device_response(line)
            except Exception as e:
                self.print_func(f"<PC> Ошибка чтения: {str(e)}")
                break
            time.sleep(0.1)

    def _process_device_response(self, line: str):
        """Обработка ответа от устройства"""
        if not self.register_queue:
            return

        # Парсим ответ регистра
        if line.startswith("<ans zsc> "):
            try:
                parts = line.split()
                response_addr = parts[2].rstrip(':')
                reg_value = parts[3]

                # Получаем ожидаемый регистр из очереди (без извлечения)
                current_reg = self.register_queue[0] if self.register_queue else None
                expected_addr = self.config['Registers'][f'reg_{current_reg}'].lower()

                # Проверяем соответствие адресов
                if response_addr.lower() == expected_addr:
                    reg_name = self.register_queue.pop(0)  # Извлекаем только при совпадении
                    self.update_register_func(reg_name, reg_value)
                    self.print_func(f"<PC> Получено значение регистра {reg_name}: {reg_value}")
                else:
                    self.print_func(
                        f"<PC> Ошибка: несоответствие адресов. Ожидался {expected_addr}, получен {response_addr}")

            except Exception as e:
                self.print_func(f"<PC> Ошибка парсинга ответа: {str(e)}")