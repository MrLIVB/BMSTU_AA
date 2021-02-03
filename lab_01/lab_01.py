from tests import *
import matplotlib.pyplot as plt

def create_fig():
    fig, axes = plt.subplots()
    axes.set_xlabel('Word length')  # Add an x-label to the axes.
    axes.set_ylabel('Elapsed time')  # Add a y-label to the axes.
    axes.set_title("Spent time")  # Add a title to the axes.
    return fig, axes

def plotter(axes, data1, data2, param_dict):
    out = axes.plot(data1, data2, **param_dict)
    return out

def main():
    while 1:
        print("Меню")
        print("1. Запустить с введёнными строками")
        print("2. Сравнить")
        print("3. Выйти")
        c = int(input("Введите вариант: "))
        if c == 1:
            s1 = input("Введите исходную строку: ")
            s2 = input("Введите реузльтирующую строку: ")
            inputed_test(s1, s2)
        
        elif c == 2:
            time_data = measure_time(20, 500, 50)
            fig, ax1 = plt.subplots(1)
            plotter(ax1, time_data[0], time_data[1][0],
                    {"label": "Матричный"})
            plotter(ax1, time_data[0], time_data[1][1], {
                    "label": "Матрично-рекурсивный"})
            plotter(ax1, time_data[0], time_data[1][2],
                    {"label": "Рекурсивный"})

            ax1.set_xlabel('Длина слов') 
            ax1.set_ylabel('Затраченное время') 
            ax1.set_title("Зависимость затрат времени от длины слова")
            plt.legend()
            plt.show()


            time_data = measure_time(20, 9, 1, True)

            fig, ax2 = plt.subplots(1)
            plotter(ax2, time_data[0], time_data[1][0],
                    {"label": "Матричный"})
            plotter(ax2, time_data[0], time_data[1][1], {
                    "label": "Матрично-рекурсивный"})
            plotter(ax2, time_data[0], time_data[1][2],
                    {"label": "Рекурсивный"})
            plotter(ax2, time_data[0], time_data[1][3],
                    {"label": "Дамерау-ЛЕвенштейна"})

            ax2.set_xlabel('Длина слов')  # Add an x-label to the axes.
            ax2.set_ylabel('Затраченное время')  # Add a y-label to the axes.
            ax2.set_title("Зависимость затрат времени от длины слова")  # Add a title to the axes.
            plt.legend()
            plt.show()


        elif c == 3:
            break

main()
