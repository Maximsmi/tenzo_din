import tkinter as tk
from tkinter import ttk

def create_input_data_frame(parent):
    # Входные данные
    ttk.Label(parent, text="Входные данные:").grid(row=0, column=0, columnspan=2, pady=5)

    ttk.Label(parent, text="F:", justify='right').grid(row=1, column=0, rowspan=3, padx=5, pady=5)
    ttk.Entry(parent, width=10, justify='right').grid(row=1, column=1, rowspan=3, padx=5, pady=5)
    ttk.Label(parent, text="кг", justify='left').grid(row=1, column=2, rowspan=3, padx=5, pady=5,  sticky="w")

    ttk.Label(parent, text="T1:", justify='right').grid(row=1, column=3, padx=5, pady=5)
    ttk.Entry(parent, width=10, justify='right').grid(row=1, column=4, padx=5, pady=5)
    ttk.Label(parent, text="С", justify='left').grid(row=1, column=5, padx=5, pady=5, sticky="w")

    ttk.Label(parent, text="T2:", justify='right').grid(row=3, column=3, padx=5, pady=5)
    ttk.Entry(parent, width=10, justify='right').grid(row=3, column=4, padx=5, pady=5)
    ttk.Label(parent, text="С", justify='left').grid(row=3, column=5, padx=5, pady=5, sticky="w")

    ttk.Label(parent, text="Uвх:", justify='right').grid(row=1, column=6, rowspan=3, padx=5, pady=5)
    ttk.Entry(parent, width=10, justify='right').grid(row=1, column=7, rowspan=3, padx=5, pady=5)
    ttk.Label(parent, text="В", justify='left').grid(row=1, column=8, rowspan=3, padx=5, pady=5, sticky="w")
