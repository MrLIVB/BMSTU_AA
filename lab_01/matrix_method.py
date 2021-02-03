def levenstein_m(origin, target):
    l1 = len(origin) + 1
    l2 = len(target) + 1
    matr = [[0 for i in range(l2)] for i in range(l1)]

    for i in range(1, l1):
        matr[i][0] = i
    for i in range(1, l2):
        matr[0][i] = i
    
    for i in range(1, l1):
        for j in range(1, l2):
            matr[i][j] = min(matr[i - 1][j] + 1, 
                             matr[i][j - 1] + 1,
                             matr[i - 1][j - 1] + (origin[i - 1] != target[j - 1]))

    return matr[l1 - 1][l2 - 1]
