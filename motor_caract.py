#!/usr/bin/env python3
import matplotlib.pyplot as plt
import control as cnt
from scipy.optimize import leastsq
import numpy as np

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

speed1 = np.loadtxt('speed1.csv')
speed2 = np.loadtxt('speed2.csv')

p0 = np.ones(2)

t = np.array([0.01 * i for i in range(100)])

p1, _ = leastsq(residual, p0, args=(t, speed1))
p2, _ = leastsq(residual, p0, args=(t, speed2))

print(p1)
print(p2)

plt.figure(1)
plt.plot(t, speed1, label='real speed1')
plt.plot(t, f(t, *p1), label='model speed1')
plt.plot(t, speed2, label='real speed2')
plt.plot(t, f(t, *p2), label='model speed2')
plt.legend()
plt.show()

k1, tau1 = p1
k2, tau2 = p2

nbStep = 1633
pwm = 220
Ts = 0.01
kp = 46.2
ki = 752

c = cnt.tf([kp, ki*Ts-kp], [1, -1], Ts)

g1d = cnt.tf(k1 / nbStep / pwm, [tau1, 1]).sample(Ts)
g2d = cnt.tf(k2 / nbStep / pwm, [tau2, 1]).sample(Ts)

y1 = cnt.feedback(c*g1d)
u1 = cnt.feedback(c, g1d)
y2 = cnt.feedback(c*g2d)
u2 = cnt.feedback(c, g2d)

t = [0.01 * i for i in range(100)]

_, stepY1 = cnt.step_response(y1, t)
_, stepY2 = cnt.step_response(y2, t)
_, stepU1 = cnt.step_response(u1, t)
_, stepU2 = cnt.step_response(u2, t)

plt.subplot(1, 2, 1)
plt.plot(t, stepY1[0], label='y1')
plt.plot(t, stepY2[0], label='y2')
plt.title('y response')

plt.subplot(1, 2, 2)
plt.plot(t, stepU1[0], label='u1')
plt.plot(t, stepU2[0], label='u2')
plt.title('u response')

plt.show()
