import tkinter as tk
from matplotlib.path import Path

CELL_SIZE = 70
GRID_SIZE = 10

class DotsGame:
    def __init__(self, root):
        self.root = root
        self.red_score = 0
        self.blue_score = 0

        self.score_label = tk.Label(root, text="Красный: 0 | Синий: 0", font=("Arial", 14))
        self.score_label.pack()

        self.canvas = tk.Canvas(root, width=GRID_SIZE * CELL_SIZE, height=GRID_SIZE * CELL_SIZE, bg='white')
        self.canvas.pack()
        self.points = []  # список: (row, col, color)
        self.blocked_points = set()  # точки, окружённые противником
        self.turn = 'red'
        self.canvas.bind('<Button-1>', self.on_click)
        self.draw_grid()

    def draw_grid(self):
        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):
                x = j * CELL_SIZE + CELL_SIZE // 2
                y = i * CELL_SIZE + CELL_SIZE // 2
                self.canvas.create_oval(x - 3, y - 3, x + 3, y + 3, fill='black')

    def update_score(self):
        self.score_label.config(text=f"Красный: {self.red_score} | Синий: {self.blue_score}")

    def on_click(self, event):
        col = event.x // CELL_SIZE
        row = event.y // CELL_SIZE
        if not self.is_occupied(row, col):
            x = col * CELL_SIZE + CELL_SIZE // 2
            y = row * CELL_SIZE + CELL_SIZE // 2
            self.points.append((row, col, self.turn))
            self.canvas.create_oval(x - 6, y - 6, x + 6, y + 6, fill=self.turn)
            captured_points = self.check_and_draw_loop((row, col), self.turn)
            for pt in captured_points:
                self.blocked_points.add(pt)
            if captured_points:
                if self.turn == 'red':
                    self.red_score += len(captured_points)
                else:
                    self.blue_score += len(captured_points)
                self.update_score()
            self.turn = 'blue' if self.turn == 'red' else 'red'

    def is_occupied(self, row, col):
        return any(p[0] == row and p[1] == col for p in self.points)

    def get_neighbors(self, row, col, color):
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1),
                      (-1, -1), (-1, 1), (1, -1), (1, 1)]
        neighbors = []
        for dr, dc in directions:
            nr, nc = row + dr, col + dc
            if 0 <= nr < GRID_SIZE and 0 <= nc < GRID_SIZE:
                if (nr, nc, color) in self.points and (nr, nc) not in self.blocked_points:
                    neighbors.append((nr, nc))
        return neighbors

    def check_and_draw_loop(self, start_point, color):
        visited = set()
        graph = { (r, c): self.get_neighbors(r, c, color)
                  for r, c, col in self.points if col == color and (r, c) not in self.blocked_points }

        def dfs(path, current, origin):
            if len(path) > 3 and current == origin:
                return path
            if current in visited:
                return None
            visited.add(current)
            for neighbor in graph.get(current, []):
                if neighbor not in path or neighbor == origin:
                    result = dfs(path + [neighbor], neighbor, origin)
                    if result:
                        return result
            visited.remove(current)
            return None

        loop = dfs([start_point], start_point, start_point)
        if loop:
            polygon = Path([
                (c * CELL_SIZE + CELL_SIZE // 2,
                 r * CELL_SIZE + CELL_SIZE // 2) for r, c in loop
            ])
            captured = []
            for r, c, col in self.points:
                if col != color and (r, c) not in self.blocked_points:
                    x = c * CELL_SIZE + CELL_SIZE // 2
                    y = r * CELL_SIZE + CELL_SIZE // 2
                    if polygon.contains_point((x, y)):
                        captured.append((r, c))
            if captured:
                for i in range(len(loop)):
                    r1, c1 = loop[i]
                    r2, c2 = loop[(i + 1) % len(loop)]
                    x1 = c1 * CELL_SIZE + CELL_SIZE // 2
                    y1 = r1 * CELL_SIZE + CELL_SIZE // 2
                    x2 = c2 * CELL_SIZE + CELL_SIZE // 2
                    y2 = r2 * CELL_SIZE + CELL_SIZE // 2
                    self.canvas.create_line(x1, y1, x2, y2, fill=color, width=2)
            return captured
        return []

root = tk.Tk()
root.title("Игра Точки: Захват и блокировка")
game = DotsGame(root)
root.mainloop()
