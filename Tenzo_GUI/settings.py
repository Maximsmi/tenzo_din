import tkinter as tk
from tkinter import ttk


class SettingsFrame:
    def __init__(self, parent):
        self.frame = ttk.Frame(parent, borderwidth=1, relief="solid")
        self.registers = {}

        # Настройки регистров zsc31050
        ttk.Label(self.frame, text="Настройки регистров zsc31050:").grid(
            row=0, column=0, columnspan=3, sticky="w", pady=5)

        self.register_names = [
            "CFGCYC", "CFGSIF", "CFGAPP", "CFGAFE",
            "CFGTMP", "CFGOUT", "ADJREF"
        ]

        for i, reg in enumerate(self.register_names):
            ttk.Label(self.frame, text=f"{reg}:", justify='right').grid(
                row=i + 1, column=1, sticky="e", padx=5, pady=2)

            entry = ttk.Entry(self.frame, width=10, justify='center')
            entry.grid(row=i + 1, column=2, padx=5, pady=2)

            ttk.Button(
                self.frame,
                text="Изменить",
                command=lambda r=reg: self._on_register_change(r)
            ).grid(row=i + 1, column=4, padx=5, pady=2)

            self.registers[reg.lower()] = entry

    def update_register_value(self, reg_name: str, value: str):
        reg_name = reg_name.lower()
        if reg_name in self.registers:
            entry = self.registers[reg_name]
            entry.delete(0, tk.END)
            entry.insert(0, value)

    def _on_register_change(self, reg_name: str):
        # Здесь будет логика изменения регистра
        print(f"Register {reg_name} changed")

    def get_widget(self):
        return self.frame

    def _validate_hex(self, value: str) -> bool:
        try:
            int(value, 16)
            return True
        except ValueError:
            return False

    def _on_register_change(self, reg_name: str):
        entry = self.registers[reg_name.lower()]
        value = entry.get()

        if not self._validate_hex(value):
            print_to_terminal(f"<PC> Ошибка: Некорректное значение для регистра {reg_name}")
            return

        # Здесь будет отправка команды изменения регистра
        print_to_terminal(f"<PC> Изменение регистра {reg_name} на значение {value}")