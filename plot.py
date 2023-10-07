import numpy as np
import matplotlib.pyplot as plt

def read_wave():
    file = open("wave.txt")
    content = (file.read()).split('\n');
    file.close()
    for i in range(len(content)):
        content[i] = content[i].split(' ')
        content[i] = [float(j) for j in content[i] if j != '']

    return content

wynik = read_wave()

wynik = [i for i in wynik if i]

x = np.linspace(-50, 50, num=len(wynik))
y = np.linspace(-50, 50, num=len(wynik[0]))
[X, Y] = np.meshgrid(x,y)

plt.title(r"Wartość funkcji falowej $\Psi$")

plt.contour(X, Y, wynik, cmap='viridis')
plt.xlabel("x[nm]")
plt.ylabel("y[nm]")
plt.colorbar()

plt.show()
