import tkinter as tk
from tkinter import ttk

def create_graph_frame(parent):
    # График
    ttk.Label(parent, text="График:").grid(row=0, column=0, sticky="w", columnspan=3, pady=5)
    canvas = tk.Canvas(parent, width=300, height=150, bg="white")
    canvas.grid(row=1, column=0, rowspan=5, padx=5, pady=5)

    # Легенда
    #ttk.Label(parent, text="F сила").grid(row=2, column=3, padx=5, pady=5)
    #ttk.Label(parent, text="T1").grid(row=3, column=3, padx=5, pady=5)
    #ttk.Label(parent, text="T2").grid(row=4, column=3, padx=5, pady=5)

    # Создаем переменную для связывания с радиокнопками
    selected_option = tk.StringVar()

    # Создаем три радиокнопки и связываем их с одной переменной
    radio1 = ttk.Radiobutton(parent, text="F сила", variable=selected_option, value="F")
    radio2 = ttk.Radiobutton(parent, text="T1", variable=selected_option, value="T1")
    radio3 = ttk.Radiobutton(parent, text="T2", variable=selected_option, value="T2")

    # Размещаем радиокнопки в окне
    radio1.grid(row=2, column=3, sticky="w", padx=5, pady=5)
    radio2.grid(row=3, column=3, sticky="w", padx=5, pady=5)
    radio3.grid(row=4, column=3, sticky="w", padx=5, pady=5)