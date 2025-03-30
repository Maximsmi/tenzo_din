import tkinter as tk
from tkinter import ttk, scrolledtext
import queue


class Terminal:
    def __init__(self, parent):
        self.frame = ttk.Frame(parent, borderwidth=1, relief="solid")
        self.message_queue = queue.Queue()

        # Заголовок терминала
        ttk.Label(self.frame, text="Терминал:").pack(pady=5, anchor="w")

        # Область для вывода сообщений
        self.text_area = scrolledtext.ScrolledText(
            self.frame,
            width=50,
            height=10,
            wrap=tk.WORD,
            state='normal'
        )
        self.text_area.pack(padx=5, pady=5, fill=tk.BOTH, expand=True)

        # Кнопка очистки терминала
        ttk.Button(
            self.frame,
            text="Очистить",
            command=self.clear
        ).pack(pady=5)

        # Запускаем обработчик очереди
        self.frame.after(100, self.process_queue)

    def clear(self):
        self.text_area.configure(state='normal')
        self.text_area.delete(1.0, tk.END)
        self.text_area.configure(state='disabled')

    def print(self, text, end="\n"):
        self.message_queue.put((text, end))

    def process_queue(self):
        while not self.message_queue.empty():
            text, end = self.message_queue.get()
            self.text_area.configure(state='normal')
            self.text_area.insert(tk.END, text + end)
            self.text_area.see(tk.END)
            self.text_area.configure(state='disabled')
        self.frame.after(100, self.process_queue)

    def get_widget(self):
        return self.frame