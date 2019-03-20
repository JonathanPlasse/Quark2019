#!/usr/bin/env python3
from scipy.optimize import leastsq
import numpy as np

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

speed1 = np.loadtxt('speed1.csv')

p0 = np.one(1)

t = [0.01 * i for i in range(1, 101)]

p = leastsq(residual, p0, args=(t, speed1))
