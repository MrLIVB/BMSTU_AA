from graph import Graph
from time import process_time

matr1 = [[-1, 6, 5, 5, 6, 9, 6, 5, 6, 8],
         [6, -1, 7, 9, 5, 8, 8, 8, 7, 7],
         [5, 7, -1, 5, 5, 6, 6, 8, 6, 9],
         [5, 9, 5, -1, 5, 8, 9, 6, 7, 8],
         [6, 5, 5, 5, -1, 8, 5, 7, 9, 7],
         [9, 8, 6, 8, 8, -1, 9, 7, 8, 9],
         [6, 8, 6, 9, 5, 9, -1, 7, 7, 5],
         [5, 8, 8, 6, 7, 7, 7, -1, 8, 8],
         [6, 7, 6, 7, 9, 8, 7, 8, -1, 9],
         [8, 7, 9, 8, 7, 9, 5, 8, 9, -1]]

matr2 = [[-1, 7, 8, 5, 6, 9, 7, 6], 
         [7, -1, 8, 8, 7, 8, 7, 8], 
         [8, 8, -1, 6, 7, 8, 8, 9], 
         [5, 8, 6, -1, 9, 6, 8, 9],
         [6, 7, 7, 9, -1, 8, 7, 6], 
         [9, 8, 8, 6, 8, -1, 5, 8], 
         [7, 7, 8, 8, 7, 5, -1, 7], 
         [6, 8, 9, 9, 6, 8, 7, -1]]

matr3 = [[-1, 5, 8, 6, 8, 8, 6, 6, 6, 6], 
         [5, -1, 8, 5, 7, 7, 9, 9, 7, 7], 
         [8, 8, -1, 9, 6, 5, 7, 5, 5, 6], 
         [6, 5, 9, -1, 8, 5, 5, 6, 9, 9], 
         [8, 7, 6, 8, -1, 6, 7, 8, 6, 8],
         [8, 7, 5, 5, 6, -1, 9, 8, 6, 6], 
         [6, 9, 7, 5, 7, 9, -1, 6, 5, 7], 
         [6, 9, 5, 6, 8, 8, 6, -1, 9, 8], 
         [6, 7, 5, 9, 6, 6, 5, 9, -1, 9], 
         [6, 7, 6, 9, 8, 6, 7, 8, 9, -1]]

def parametrization():
    g = Graph(10, matr1)
    g2 = Graph(10, matr3)

    reference1 = g.ets()
    reference_l1 = g.count_cost(reference1)

    reference2 = g2.ets()
    reference_l2 = g2.count_cost(reference2)

    print(reference_l1)
    print(reference_l2)
    f = open("parametrisation.txt", 'w')
    for herd in range(0, 10, 2):
        for greed in range(0, 10, 2):
            evaporation = 0.1
            while evaporation < 1:
                for days in range(10, 51, 10):
                    f.write("{:2d} {:.1f} {:2d} ".format(herd, evaporation, days))

                    rout1 = g.ant_solve(herd, greed, evaporation, days)
                    rout2 = g.ant_solve(herd, greed, evaporation, days)
                    l11 = g.count_cost(rout1)
                    l12 = g.count_cost(rout2)
                    tl1 = min(l11, l12)
                    l1 = reference_l1 - tl1

                    rout1 = g2.ant_solve(herd, greed, evaporation, days)
                    rout2 = g2.ant_solve(herd, greed, evaporation, days)
                    l21 = g2.count_cost(rout1)
                    l22 = g2.count_cost(rout2)
                    tl2 = min(l21, l22)
                    l2 = reference_l2 - tl2
                    
                    f.write("{:2d} {:2d} {:2d} {:2d}\n".format(tl1, tl2, l1, l2))

                evaporation += 0.2



if __name__ == "__main__":
    parametrization()
    def time():
        f = open("report\\src\\time.txt", 'w')
        for s in range(3,11):
            g = Graph(s)
            g.fill_random()
            t1_start = process_time()
            for i in range(50):
                g.ets()
            t1_end = process_time()
            t1 = (t1_end-t1_start)/50

            t2_start = process_time()
            for i in range(50):
                g.ant_solve(4, 6, 0.5, 20)
            t2_end = process_time()
            t2 = (t2_end-t2_start)/50

            f.write(f"{s} {t1} {t2}\n")
            print(f"Elapsed time ets={t1}, ant={t2}")
        
