import tkinter as tk

CELL_SIZE = 70
GRID_SIZE = 10

class DotsGame:
    def __init__(self, root):
        self.root = root
        self.canvas = tk.Canvas(root, width=GRID_SIZE*CELL_SIZE, height=GRID_SIZE*CELL_SIZE, bg='white')
        self.canvas.pack()
        self.points = []
        self.turn = 'red'
        self.canvas.bind('<Button-1>', self.on_click)
        self.draw_grid()

    def draw_grid(self):
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):
                x = j * CELL_SIZE + CELL_SIZE // 2
                y = i * CELL_SIZE + CELL_SIZE // 2
                self.canvas.create_oval(x-3, y-3, x+3, y+3, fill='black')

    def on_click(self, event):
        col = event.x // CELL_SIZE
        row = event.y // CELL_SIZE
        x = col * CELL_SIZE + CELL_SIZE // 2
        y = row * CELL_SIZE + CELL_SIZE // 2
        if (row, col) not in self.points:
            self.points.append((row, col))
            self.canvas.create_oval(x-6, y-6, x+6, y+6, fill=self.turn)
            self.turn = 'blue' if self.turn == 'red' else 'red'

root = tk.Tk()
root.title("Игра Точки")
game = DotsGame(root)
root.mainloop()
