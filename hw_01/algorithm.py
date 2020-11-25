def alg(matr, rows, cols):
    avg = 0                         # 1
    avg_cnt = 0                     # 2
    for i in range(rows):
        for j in range(cols):
            avg = avg + matr[i][j]  # 3
            avg_cnt = avg_cnt + 1   # 4
    avg = avg / avg_cnt             # 5
    max_el = matr[0][0] * avg       # 6
    max_el_cnt = 0                  # 7
    for i in range(rows):
        for j in range(cols):
            matr[i][j] = matr[i][j] * avg # 8
            if matr[i][j] > max_el:       #9
                max_el = matr[i][j] # 10
                max_el_cnt = 1      # 11
            elif matr[i][j] == max_el: # 12
                max_el_cnt += 1     # 13
    return max_el_cnt               # 14


matr = [[30, -12, 3, -21], [-3, -5, -6, 2], [-21, -21, -1, 0]]
print(alg(matr, 3, 4))
