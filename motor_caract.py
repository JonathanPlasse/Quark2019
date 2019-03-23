#!/usr/bin/env python3
import matplotlib.pyplot as plt
import control as cnt
from scipy.optimize import leastsq
import numpy as np

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

speed = np.loadtxt('speed1.csv')

p0 = np.ones(2)

t = np.array([0.01 * i for i in range(100)])

p, _ = leastsq(residual, p0, args=(t, speed))

k1, tau1 = p

nbStep = 1633
pwm = 220
Ts = 0.01
kp = 46.2
ki = 752

c = cnt.tf([kp, ki*Ts-kp], [1, -1], Ts)

gd = cnt.tf(k1 / nbStep / pwm, [tau1, 1]).sample(Ts)

y = cnt.feedback(c*gd)
u = cnt.feedback(c, gd)

t = [0.01 * i for i in range(100)]

_, stepY = cnt.step_response(y, t)
_, stepU = cnt.step_response(u, t)

plt.subplot(131)
plt.plot(t, speed, label='real speed')
plt.plot(t, f(t, *p), label='model speed')
plt.title('Open-loop')
plt.legend()

plt.subplot(132)
plt.plot(t, stepY[0], label='Y')
plt.title('Y response')
plt.legend()

plt.subplot(133)
plt.plot(t, stepU[0], label='U')
plt.title('U response')
plt.legend()

plt.show()
