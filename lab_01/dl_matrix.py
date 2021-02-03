from numpy import zeros, int8

def dl_matrix(origin, target):
    l1 = len(origin) + 1
    l2 = len(target) + 1
    matrix = [[0 for i in range(l2)] for i in range(l1)]

    for i in range(1, l1):
        matrix[i][0] = i
    for i in range(1, l2):
        matrix[0][i] = i

    for i in range(1, l1):
        for j in range(1, l2):
            if (i > 1 and j > 1) and (origin[i - 2] == target[j - 1] and origin[i - 1] == target[j - 2]):
                switch = matrix[i - 2][j - 2] + 1
                matrix[i][j] = min(matrix[i - 1][j] + 1,
                                   matrix[i][j - 1] + 1,
                                   matrix[i - 1][j - 1] + (origin[i - 1] != target[j - 1]),
                                   switch)
            else:                    
                matrix[i][j] = min(matrix[i - 1][j] + 1,
                                matrix[i][j - 1] + 1,
                                matrix[i - 1][j - 1] + (origin[i - 1] != target[j - 1]))

    return matrix[l1 - 1][l2 - 1]


