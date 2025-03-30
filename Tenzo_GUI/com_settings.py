# com_settings.py
import tkinter as tk
from tkinter import ttk, messagebox
import configparser


def show_com_settings(parent, config):
    # Создаем окно настроек
    settings_window = tk.Toplevel(parent)
    settings_window.title("Настройки COM-порта")

    # Переменные для хранения настроек
    port_var = tk.StringVar(value=config['COM']['port'])
    baudrate_var = tk.StringVar(value=config['COM']['baudrate'])

    # Создаем элементы интерфейса
    ttk.Label(settings_window, text="Порт:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
    port_entry = ttk.Entry(settings_window, textvariable=port_var)
    port_entry.grid(row=0, column=1, padx=5, pady=5)

    ttk.Label(settings_window, text="Скорость:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
    baudrate_entry = ttk.Entry(settings_window, textvariable=baudrate_var)
    baudrate_entry.grid(row=1, column=1, padx=5, pady=5)

    def save_settings():
        # Проверяем введенные данные
        if not port_var.get() or not baudrate_var.get():
            messagebox.showerror("Ошибка", "Все поля должны быть заполнены")
            return

        try:
            int(baudrate_var.get())
        except ValueError:
            messagebox.showerror("Ошибка", "Скорость должна быть числом")
            return

        # Сохраняем настройки в конфиг
        config['COM']['port'] = port_var.get()
        config['COM']['baudrate'] = baudrate_var.get()

        # Записываем изменения в файл
        with open('config.ini', 'w', encoding='utf-8') as configfile:
            config.write(configfile)

        messagebox.showinfo("Успех", "Настройки сохранены")
        settings_window.destroy()

    # Кнопки сохранения и отмены
    ttk.Button(settings_window, text="Сохранить", command=save_settings).grid(row=2, column=0, columnspan=2, pady=10)
    ttk.Button(settings_window, text="Отмена", command=settings_window.destroy).grid(row=3, column=0, columnspan=2)