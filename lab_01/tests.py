from matrix_method import levenstein_m
from recursive_method import levenstein_r
from recmat_method import levenstein_rm
from dl_matrix import dl_matrix

from random import randint, random
from time import process_time


def inputed_test(s1, s2):
    print("Реузльтат, используя матричный метод: ", levenstein_m(s1, s2))

    print("Реузльтат, используя рекурсивный метод: ", levenstein_r(s1, s2))

    print("Реузльтат, используя матрично-рекурсивный метод: ", levenstein_rm(s1, s2))

    print("Реузльтат расстояния Дамерлау-Левенштейна: ", dl_matrix(s1, s2))


def rand_generator(l):
    s1 = ""
    s2 = ""
    i = 0
    while i < l:
        s1 += str(randint(0, 9))
        s2 += str(randint(0, 9))
        i += 1
    return s1, s2

def generate_strings(n, l, generator):
    origins = []
    targets = []
    for i in range(n):
        t1, t2 = generator(l)
        origins.append(t1)
        targets.append(t2)

    return origins, targets

def measure_time_method(origins, targets, method):
    l = len(origins)
    elapsed_time = 0
    for i in range(l):
        s1 = origins[i]
        s2 = targets[i]

        start = process_time()
        method(s1, s2)
        end = process_time()

        elapsed_time += end - start

    return elapsed_time / l

def measure_time(repeats, max_length, step, measure_recursion_flag=False):
    if measure_recursion_flag:
        methods = [levenstein_m, levenstein_rm, levenstein_r, dl_matrix]
        method_results = [[], [], [], []]
    else:
        methods = [levenstein_m, levenstein_rm, dl_matrix]
        method_results = [[], [], []]

    lengthes = []

    for i in range(1, max_length, step):
        origins, targets = generate_strings(repeats, i, rand_generator)
        lengthes.append(i)

        for j in range(len(methods)):
            method_results[j].append(
                measure_time_method(origins, targets, methods[j]))

    return lengthes, method_results

# def measure_memory_method(origins, targets, method):
#     l = len(origins)
#     peak_memory = 0
#     for i in range(l):
#         s1 = origins[i]
#         s2 = targets[i]

#         tracemalloc.start()
#         method(s1, s2)
#         current, peak = tracemalloc.get_traced_memory()
#         internal = tracemalloc.get_tracemalloc_memory()
#         tracemalloc.stop()

#         peak_memory += peak - internal

#     return peak_memory / l

# def measure_memory(repeats):
#     methods = [levenstein_m, levenstein_rm, dl_matrix]

#     method_results = [[], [], []]
#     lengthes = []

#     for i in range(1, 11):
#         origins, targets = generate_strings(repeats, i*10, rand_generator)
#         lengthes.append(i*10)

#         for j in range(len(methods)):
#             method_results[j].append(
#                 measure_memory_method(origins, targets, methods[j]))

#     return lengthes, method_results
