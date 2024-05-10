import sys
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib import cm

def make_3Dplot(time, x, u):
    fig = plt.figure()
    ax = plt.axes(projection ='3d')
    x, time = np.meshgrid(x, time)
    ax.plot_surface(x, time, np.array(u), cmap=cm.coolwarm)
    ax.set_title('3D representation')
    ax.set_ylabel('x')
    ax.set_xlabel('t')
    ax.set_zlabel('u')
    plt.show()
    plt.close()

def color_fader(c1='blue', c2='red', mix=0):
    c1=np.array(mpl.colors.to_rgb(c1))
    c2=np.array(mpl.colors.to_rgb(c2))
    return mpl.colors.to_hex((1-mix)*c1 + mix*c2)

def make_2Dplot(time, x, u):
    for ind in range(len(time)):
        plt.plot(x, u[ind], color=color_fader(mix=ind/len(time)))
    norm = mpl.colors.Normalize(vmin=0, vmax=max(time))
    sm = plt.cm.ScalarMappable(cmap=cm.coolwarm, norm=norm)
    plt.colorbar(sm, pad=0.04, label="Time, s")
    plt.show() 
    plt.close()

if __name__ == '__main__':
    if len(sys.argv) < 2:
        file = 'results.txt'
    else:
        file = sys.argv[1]
    u = []
    with open(file, 'r') as dump:
        lines = dump.readlines()
        for line in lines:
            u.append(np.fromstring(line, dtype=np.double, sep=' '))
    time = u[0]
    x = u[1]
    u = u[2:]
    make_2Dplot(time, x, u)
    make_3Dplot(time, x, u)
