from time import process_time_ns
from itertools import permutations
from random import random, randrange, uniform

class Graph():
    def __init__(self, _n, _c=None):
        self.nodes = _n
        self.c = _c

    def ets(self):
        start = [i for i in range(1, self.nodes)]
        min_len = float("inf")
        min_rout = [0 for _ in range(self.nodes)]
        routs = permutations(start)
        for rout in routs:
            rout = list(rout)
            rout.insert(0, 0)
            rout.append(0)
            cur_len = self.count_cost(rout)
            if cur_len < min_len:
                min_len = cur_len
                min_rout = rout
        return min_rout

    def ant_solve(self, herd, greed, evaportion, tmax):
        # Задание начальных матриц
        pheromones = [[0.1 for _ in range(self.nodes)] for _ in range(self.nodes)]
        visibility = [[1 / self.c[j][i] for j in range(self.nodes)] for i in range(self.nodes)]
        q = self.calculate_q()

        min_len = 0
        for i in range(1,self.nodes):
            min_len += self.c[i-1][i]
        min_len += self.c[self.nodes-1][0]
        min_rout = [i for i in range(self.nodes)]
        min_rout.append(0)

        # Инициализация муравьев
        possibilities = [i for i in range(self.nodes)]
        ants = [Ant(i,possibilities,herd,greed) for i in range(self.nodes)]

        # Цикл по времени жизни колонии
        for i in range(tmax):
            # Цикл по всем муравьям
            delta_pher = 0
            for i in range(self.nodes):
                ants[i].find_way(pheromones,visibility)
                l = self.count_cost(ants[i].rout)
                ph = q / l
                delta_pher += ph

                if l < min_len:
                    min_len = l
                    min_rout = [ants[i].rout[j] for j in range(len(ants[i].rout))]
                
                ants[i].reset(possibilities)

            for i in range(self.nodes):
                for j in range(self.nodes):
                    if i != j:
                        pheromones[i][j] *= (1 - evaportion)
                        pheromones[i][j] += delta_pher
        
        return min_rout
    
    def count_cost(self, rout):
        cur_len = 0
        for i in range(1, self.nodes + 1):
            cur_len += self.c[rout[i-1]][rout[i]]
        return cur_len

    def calculate_q(self):
        q = 0
        for i in range(self.nodes):
            for j in range(self.nodes):
                q += self.c[i][j] if i != j else 0
        return q / self.nodes


    def fill_random(self):
        if self.c == None:
            self.c = [[0 for _ in range(self.nodes)] for _ in range(self.nodes)]
        for i in range(self.nodes):
            for j in range(self.nodes):
                if i < j:
                    self.c[i][j] = randrange(5,10)
                elif i > j:
                    self.c[i][j] = self.c[j][i]
                else:
                    self.c[i][j] = -1


class Ant():
    def __init__(self, start, possibilities, _herd, _greed):
        self.possibilities = [possibilities[i] for i in range(len(possibilities))]
        self.possibilities.remove(start)
        self.base_pos = start
        self.pos = start
        self.herd = _herd
        self.greed = _greed
        self.rout = []
        self.rout.append(start)

    def find_way(self, pheromones, visibility):
        while 1:
            chances = []
            zn = 0
            for city in self.possibilities:
                t = pow(pheromones[self.pos][city], self.herd) * pow(visibility[self.pos][city], self.greed)
                zn += t
                chances.append([city, t])
            for i in range(len(self.possibilities)):
                chances[i][1] = chances[i][1] / zn

            city = self.make_choice(chances)
            self.pos = city
            self.possibilities.remove(city)
            self.rout.append(city)
            if len(self.possibilities) == 0:
                break
        
        self.rout.append(self.base_pos)
    
    def make_choice(self, chances):
        chances_sum = 0
        for chance in chances:
            chances_sum += chance[1]
        choice = uniform(0, chances_sum)     
        next_city = 0
        
        t = 0
        while choice > 0:
            choice -= chances[t][1]
            t += 1
        next_city = chances[t-1][0]
        return next_city

    def reset(self, possibilities):
        self.possibilities = [possibilities[i] for i in range(len(possibilities))]
        self.possibilities.remove(self.base_pos)
        self.pos = self.base_pos
        self.rout = [self.base_pos]


if __name__ == "__main__":
    g = Graph(5)
    g.fill_random()
    l1 = float("inf")
    r1 =[]
    for i in range(0, 10):
        r = g.ant_solve(6, i, 0.5, 50)
        l = g.count_cost(r)
        if l < l1:
            l1 = l
            r1 = r
            print(i)
    r2 = g.ets()
    print(g.count_cost(r1), r1)
    print(g.count_cost(r2), r2)
