import tkinter as tk
from tkinter import ttk

def create_nrf24l01_settings_frame(parent):
    frame = ttk.Frame(parent, borderwidth=1, relief="solid")

    # Настройки nrf24L01
    ttk.Label(frame, text="Настройки nrf24L01:").grid(row=0, column=0, columnspan=3, sticky="w", pady=5)
    ttk.Label(frame, text="Адрес:").grid(row=1, column=0, rowspan=3, padx=5, pady=5)
    address_entry = ttk.Entry(frame, width=30)
    address_entry.grid(row=1, column=1, rowspan=3, padx=5, pady=5)

    # Кнопки для включения/выключения устройства
    #ttk.Button(frame, text="Включить устройство").grid(row=2, column=0, padx=5, pady=5)
    #ttk.Button(frame, text="Выключить устройство").grid(row=2, column=1, padx=5, pady=5)

    # Создаем переменную для связывания с радиокнопками
    selected_option_nrf = tk.StringVar()

    # Создаем три радиокнопки и связываем их с одной переменной
    radio1 = ttk.Radiobutton(frame, text="Включить устройство", variable=selected_option_nrf, value="nrf_on")
    radio2 = ttk.Radiobutton(frame, text="Выключить устройство", variable=selected_option_nrf, value="nrf_off")

    # Размещаем радиокнопки в окне
    radio1.grid(row=1, column=3, sticky="w", padx=5, pady=5)
    radio2.grid(row=3, column=3, sticky="w", padx=5, pady=5)

    # Кнопка сохранения
    ttk.Button(frame, text="Сохранить").grid(row=5, column=3, columnspan=3, pady=5)

    return frame
