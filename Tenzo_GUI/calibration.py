import tkinter as tk
from tkinter import ttk

def create_calibration_frame(parent):
    # Тарировочные коэффициенты
    ttk.Label(parent, text="Тарировочные коэффициенты:").grid(row=0, column=0, columnspan=10, pady=5, sticky="w")
    for i in range(10):
        for j in range(10):
            ttk.Entry(parent, width=5).grid(row=i+1, column=j, padx=1, pady=1)

    ttk.Button(parent, text="Калибровка").grid(row=11, column=0, columnspan=10, pady=5)
