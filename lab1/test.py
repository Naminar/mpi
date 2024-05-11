import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from matplotlib import cm
import math 
fig, ax = plt.subplots()

for ind in range(10):
    x    = [i for i in range(ind)]
    y    = [math.sin(i + ind) for i in range(ind)]
    cols = np.linspace(0,ind,len(x))

    points = np.array([x, y]).T.reshape(-1, 1, 2)
    segments = np.concatenate([points[1:], points[:-1]], axis=1)

    lc = LineCollection(segments, cmap=cm.coolwarm)
    lc.set_array(cols)
    lc.set_linewidth(2)
    line = ax.add_collection(lc)
    ax.autoscale()
fig.colorbar(line,ax=ax, size=0.2)    
plt.show()
# def multiline(xs, ys, c, ax=None, **kwargs):
#     """Plot lines with different colorings

#     Parameters
#     ----------
#     xs : iterable container of x coordinates
#     ys : iterable container of y coordinates
#     c : iterable container of numbers mapped to colormap
#     ax (optional): Axes to plot on.
#     kwargs (optional): passed to LineCollection

#     Notes:
#         len(xs) == len(ys) == len(c) is the number of line segments
#         len(xs[i]) == len(ys[i]) is the number of points for each line (indexed by i)

#     Returns
#     -------
#     lc : LineCollection instance.
#     """

#     # find axes
#     ax = plt.gca() if ax is None else ax

#     # create LineCollection
#     segments = [np.column_stack([x, y]) for x, y in zip(xs, ys)]
#     lc = LineCollection(segments, **kwargs)

#     # set coloring of line segments
#     #    Note: I get an error if I pass c as a list here... not sure why.
#     lc.set_array(np.asarray(c))

#     # add lines to axes and rescale 
#     #    Note: adding a collection doesn't autoscalee xlim/ylim
#     ax.add_collection(lc)
#     ax.autoscale()
#     return lc


# n_lines = 30
# x = np.arange(100)

# yint = np.arange(0, n_lines*10, 10)
# ys = np.array([x + b for b in yint])
# xs = np.array([x for i in range(n_lines)]) # could also use np.tile

# colors = np.arange(n_lines)

# fig, ax = plt.subplots()
# lc = multiline(xs, ys, yint, cmap='bwr', lw=2)

# axcb = fig.colorbar(lc)
# axcb.set_label('Y-intercept')
# ax.set_title('Line Collection with mapped colors')
# plt.show()
