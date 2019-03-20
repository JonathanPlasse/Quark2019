#!/usr/bin/env python3
from scipy.optimize import leastsq
import numpy as np
import matplotlib.pyplot as plt

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

speed1 = np.loadtxt('speed1.csv')
speed2 = np.loadtxt('speed2.csv')

p0 = np.array([9000, 0.15])

t = np.array([0.01 * i for i in range(100)])

p1, _ = leastsq(residual, p0, args=(t, speed1))
p2, _ = leastsq(residual, p0, args=(t, speed2))

print(p1)
print(p2)

plt.figure(1)
plt.plot(t, speed1)
plt.plot(t, f(t, *p1))

plt.figure(2)
plt.plot(t, speed2)
plt.plot(t, f(t, *p2))

plt.show()
