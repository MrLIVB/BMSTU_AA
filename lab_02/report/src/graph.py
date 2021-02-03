import matplotlib.pyplot as plt

fig, ax1 = plt.subplots(1)

# filename1 = "standardEven.txt"
# filename2 = "winogradEven.txt"
# filename3 = "winoptimEven.txt"

filename1 = "standardOdd.txt"
filename2 = "winogradOdd.txt"
filename3 = "winoptimOdd.txt"

f = open(filename1, 'r')

x = []
y = []

for i in range(10):
    d = f.readline().split()
    x.append(int(d[0]))
    print(d[0])
    y.append(float(d[1]))

f = open(filename2, 'r')
ax1.plot(x, y, label="Стандартный")

x.clear()
y.clear()

for i in range(10):
    d = f.readline().split()
    print(d[0])
    x.append(int(d[0]))
    y.append(float(d[1]))
ax1.plot(x, y, label="Винограда")

f = open(filename3, 'r')
x.clear()
y.clear()
for i in range(10):
    d = f.readline().split()
    print(d[0])
    x.append(int(d[0]))
    y.append(float(d[1]))
ax1.plot(x, y, label="Оптимизированный Винограда")

ax1.set_xlabel('Размер матрицы') 
ax1.set_ylabel('Затраченное время') 
ax1.set_title("Зависимость затрат времени размера слова")
plt.legend()
plt.show()