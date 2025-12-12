from random import seed
from matplotlib import projections
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

# 1
x = np.arange(-np.pi, np.pi, 0.1)
y = np.sin(x)

plt.plot(x, y, 'b')
plt.show()

# 2
x = np.arange(-5, 5, 0.1)
y = x**2
plt.xlim(-5, 5)
plt.ylim(0, 100)
plt.xlabel('x')
plt.ylabel('y=x^2')
plt.title('Plot y=x^2')
plt.plot(x, y)
plt.show()

# 3
x = np.linspace(-np.pi, np.pi, 100)
cos = np.cos(x)
plt.xticks(np.linspace(-np.pi, np.pi, 5))
plt.plot(x, cos, color='red', linewidth=2.0, linestyle='-')
plt.show()

# 4
x = np.linspace(-np.pi, np.pi, 100)
sin, cos = np.sin(x), np.cos(x)
plt.plot(x, sin, color='blue', linewidth=2.0, linestyle='-', label='sin')
plt.plot(x, cos, color='red', linewidth=2.0, linestyle='--', label='cos')
plt.legend(loc='upper left')
plt.show()

# 5
data = np.random.randint(1, 11, 5)
plt.pie(data, explode=[0, 0, 0.2, 0, 0])
plt.show()

# 6
n = 100
x = np.random.normal(0, 1, n)
y = np.random.normal(0, 1, n)
plt.scatter(x, y)
plt.show()

# 7
mpl.rcParams['font.family'] = 'sans-serif'
mpl.rcParams['font.sans-serif'] = [u'SimHei']
plt.rcParams['axes.unicode_minus'] = False
data = np.random.randint(1, 10, 10)
x = np.arange(len(data))
plt.plot(x, data, color='r')
plt.bar(x, data, alpha=0.5, color='b', width=0.2)
plt.show()

# 8
x = np.linspace(0, 5, 10)
y = x**2
plt.subplot(1, 2, 1)
plt.plot(x, y, 'r--')
plt.subplot(1, 2, 2)
plt.plot(y, x, 'g*-')
plt.show()

plt.subplot(2, 1, 1)
plt.plot(x, y, 'r--')
plt.subplot(2, 1, 2)
plt.plot(y, x, 'g*-')
plt.show()

# 9
x = np.linspace(0, 5, 10)
y = x**2
fig, axis = plt.subplots(figsize=(8, 6), dpi=100)
axis.plot(x, y, 'b-')
axis.set_xlabel('x')
axis.set_ylabel('y')
axis.set_title('title')
plt.show()

# 10
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
X = [0, 1, 2, 1.5]
Y = [0, 4, 4, 1]
Z = [0, 2, 0, 0]
ax.plot_trisurf(X, Y, Z)
plt.show()

# 11
n_angles = 36
n_radii = 8
radii = np.linspace(0.125, 1.0, n_radii)
angles = np.linspace(0, 2 * np.pi, n_angles, endpoint=False)
angles = np.repeat(angles[..., np.newaxis], n_radii, axis=1)
x = np.append(0, (radii * np.cos(angles)).flatten())
y = np.append(0, (radii * np.sin(angles)).flatten())
z = np.sin(-x * y)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_trisurf(x, y, z, cmap=plt.get_cmap('rainbow'), linewidth=0.2)
plt.show()

# 12


def bivariate_normal(X, Y, sigmax=1.0, sigmay=1.0, mux=0.0, muy=0.0):
    Xmu = X - mux
    Ymu = Y - muy
    return (1.0 / (2.0 * np.pi * sigmax * sigmay)) * np.exp(
        - (Xmu**2 / (2.0 * sigmax**2) + Ymu**2 / (2.0 * sigmay**2))
    )


delta = 0.025
x = np.arange(-3.0, 3.0, delta)
y = np.arange(-2.0, 2.0, delta)
X, Y = np.meshgrid(x, y)

Z1 = bivariate_normal(X, Y, 1.0, 1.0, 0.0, 0.0)
Z2 = bivariate_normal(X, Y, 1.5, 0.5, 1.0, 0.0)

Z = 10.0 * (Z2 - Z1)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(X, Y, Z, cmap=plt.get_cmap('rainbow'), linewidth=0.2)
plt.show()

# 13
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
x_surf = np.arange(0, 1, 0.01)
y_surf = np.arange(0, 1, 0.01)
x_surf, y_surf = np.meshgrid(x_surf, y_surf)
z_surf = np.sqrt(x_surf + y_surf)
ax.plot_surface(x_surf, y_surf, z_surf)
n = 100
seed(0)
x = [np.random.rand() for _ in range(n)]
y = [np.random.rand() for _ in range(n)]
z = [np.random.rand() for _ in range(n)]
ax.scatter(x, y, z, c='r', marker='o')
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
plt.show()

# 14
plt.figure()
CS = plt.contour(X, Y, Z, 10)
plt.clabel(CS, inline=1, fontsize=10)
plt.title('Simplest default with labels')
plt.show()
