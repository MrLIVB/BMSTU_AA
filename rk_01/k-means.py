from tkinter import *
from random import randint
from math import sqrt

canvas_width = 500
canvas_height = 500

COLORS1 = ['blue', 'cyan', 'lime green', 'yellow', 'red', 'purple', 'black', 'steel blue', 'orange', 'gold', 'rosy brown']

def draw_point_b(x, y, color='black'):
    x1, y1 = (x - 3), (y - 3)
    x2, y2 = (x + 3), (y + 3)
    w.create_oval(x1, y1, x2, y2, fill=color)

def draw_point(x, y, color='black'):
    x1, y1 = (x - 2), (y - 2)
    x2, y2 = (x + 2), (y + 2)
    w.create_oval(x1, y1, x2, y2, fill=color)


master = Tk()
master.title("K-means")
w = Canvas(master,
           width=canvas_width,
           height=canvas_height)
w.pack(expand=YES, fill=BOTH)

def dist(p1, p2):
    return sqrt((p1[0] - p2[0])*(p1[0] - p2[0]) + (p1[1] - p2[1])*(p1[1] - p2[1]))

def clustering(points, k):
    centers = []
    clusters = []
    # Инициализация центров кластеров
    for _ in range(k):
        centers.append([randint(0, canvas_width), randint(0, canvas_height)])

    end = 0
    max_iterations = 100
    iteration = 0
    while not end:
        # Снова расчитываем кластеры
        clusters.clear()
        for _ in range(k):
            clusters.append([])
        for point in points:
            mind = float("inf")
            mind_i = 0
            for i in range(k):
                d = dist(centers[i], point)
                if d < mind:
                    mind_i = i
                    mind = d

            clusters[mind_i].append(point)
        # Рассчитываем новые центры как среднее
        for i in range(k):
            center_x = 0
            center_y = 0

            for point in clusters[i]:
                center_x += point[0]
                center_y += point[1]

            if len(clusters[i]):
                center_x /= len(clusters[i])
                center_y /= len(clusters[i])
                centers[i] = [center_x, center_y]

        iteration += 1
        if iteration == max_iterations:
            end = 1

    return centers, clusters


points = []
for i in range(100):
    point = [randint(0, canvas_width), randint(0, canvas_height)]
    points.append(point)

k = 5
centers, clusters = clustering(points, k)
print(centers)
for cluster in clusters:
    print(cluster)

for i in range(k):
    draw_point_b(centers[i][0], centers[i][1], color=COLORS1[i])
    for point in clusters[i]:
        draw_point(point[0], point[1], color=COLORS1[i])





mainloop()