from numpy import full

def _rec(s1, s2, matr, i, j):
    if not i:
        matr[i][j] = j
    elif not j:
        matr[i][j] = i

    elif matr[i][j] == -1:
        matr[i][j] = min(_rec(s1, s2, matr, i - 1, j) + 1, 
                         _rec(s1, s2, matr, i, j - 1) + 1,
                         _rec(s1, s2, matr, i - 1, j - 1) + int(s1[i - 1] != s2[j - 1]))

    return matr[i][j]


def levenstein_rm(origin, target):
    l1 = len(origin) + 1
    l2 = len(target) + 1
    matr = [[-1 for i in range(l2)] for i in range(l1)]

    _rec(origin, target, matr, l1 - 1, l2 - 1)
    return matr[l1 - 1][l2 - 1]

