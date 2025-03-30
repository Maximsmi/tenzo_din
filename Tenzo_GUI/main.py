import tkinter as tk
from tkinter import ttk
import configparser
import serial
from input_data import create_input_data_frame
from graph import create_graph_frame
from calibration import create_calibration_frame
from settings import SettingsFrame
from nrf24l01_settings import create_nrf24l01_settings_frame
from terminal import Terminal
from com_handler import ComHandler

# Чтение конфигурационного файла
config = configparser.ConfigParser()
config.read('config.ini', encoding='utf-8')

# Создание главного окна
root = tk.Tk()
root.title("Приложение для сбора данных")

# Установка шрифта для меню
menu_font = ("Arial", 10)

# Создание меню
menu = tk.Menu(root, font=menu_font)
root.config(menu=menu)

# Меню "Выход"
exit_menu = tk.Menu(menu, tearoff=0, font=menu_font)
exit_menu.add_command(label=config['Menu']['exit'], command=root.quit)
menu.add_cascade(label=config['Menu']['exit'], menu=exit_menu)

# Меню "Соединение"
connection_menu = tk.Menu(menu, tearoff=0, font=menu_font)
menu.add_cascade(label=config['Menu']['connection'], menu=connection_menu)

# Меню "Настройки"
settings_menu = tk.Menu(menu, tearoff=0, font=menu_font)
settings_menu.add_command(label=config['Menu']['connection_settings'])

zsc31050_menu = tk.Menu(settings_menu, tearoff=0, font=menu_font)
zsc31050_menu.add_command(label=config['Menu']['cfgcyc'])
zsc31050_menu.add_command(label=config['Menu']['cfgsif'])
zsc31050_menu.add_command(label=config['Menu']['cfgapp'])
zsc31050_menu.add_command(label=config['Menu']['cfgafe'])
zsc31050_menu.add_command(label=config['Menu']['cfgtmp'])
zsc31050_menu.add_command(label=config['Menu']['cfgout'])
zsc31050_menu.add_command(label=config['Menu']['adjref'])
settings_menu.add_cascade(label=config['Menu']['zsc31050_settings'], menu=zsc31050_menu)

settings_menu.add_command(label=config['Menu']['nrf24l01_settings'])
menu.add_cascade(label=config['Menu']['settings'], menu=settings_menu)

# Основная область интерфейса
main_frame = ttk.Frame(root)
main_frame.pack(fill=tk.BOTH, expand=True)

# Создание областей интерфейса
input_data_frame = ttk.Frame(main_frame, borderwidth=1, relief="solid")
input_data_frame.grid(row=0, column=0, padx=2, pady=2, sticky="nsew")

graph_frame = ttk.Frame(main_frame, borderwidth=1, relief="solid")
graph_frame.grid(row=1, column=0, padx=2, pady=2, sticky="nsew")

calibration_frame = ttk.Frame(main_frame, borderwidth=1, relief="solid")
calibration_frame.grid(row=2, column=0,rowspan=2,  padx=2, pady=2, sticky="nsew")

#settings_frame = ttk.Frame(main_frame, borderwidth=1, relief="solid")
#settings_frame.grid(row=1, column=1, rowspan=2, padx=2, pady=2, sticky="nsew")


# Создаем терминал
terminal = Terminal(main_frame)
terminal_frame = terminal.get_widget()
terminal_frame.grid(row=3, column=1, rowspan=2, padx=2, pady=2, sticky="nsew")

# Настройка сетки
main_frame.grid_rowconfigure(0, weight=1)
main_frame.grid_rowconfigure(1, weight=1)
main_frame.grid_rowconfigure(2, weight=1)
main_frame.grid_columnconfigure(0, weight=1)
main_frame.grid_columnconfigure(1, weight=1)
main_frame.grid_columnconfigure(2, weight=1)

# Добавление виджетов в области
create_input_data_frame(input_data_frame)
create_graph_frame(graph_frame)
create_calibration_frame(calibration_frame)
settings = SettingsFrame(main_frame)
settings_frame = settings.get_widget()
settings_frame.grid(row=1, column=1, rowspan=2, padx=2, pady=2, sticky="nsew")
#settings = SettingsFrame(settings_frame)
#settings_frame = settings.get_widget()

# Добавление панели настройки nrf24l01
nrf24l01_frame = create_nrf24l01_settings_frame(main_frame)
nrf24l01_frame.grid(row=0, column=1, padx=2, pady=2, sticky="nsew")

# Инициализация COM-обработчика
#com_handler = ComHandler(config, print_to_terminal, settings.update_register_value)
com_handler = ComHandler(
    config,
    lambda text, end="\n": terminal.print(text, end),
    settings.update_register_value
)

# Обновление меню подключения
connection_menu.add_command(
    label=config['Menu']['connect'],
    command=lambda: com_handler.connect()
)
connection_menu.add_command(
    label=config['Menu']['disconnect'],
    command=lambda: com_handler.disconnect()
)

# Запуск главного цикла
root.mainloop()