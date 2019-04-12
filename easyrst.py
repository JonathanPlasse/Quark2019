#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QVBoxLayout, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg\
                                        as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT\
                                        as NavigationToolbar
from matplotlib.figure import Figure
import control as cnt
import numpy as np
from numpy.polynomial import polynomial as P


def zero(z0):
    """Return a monic zero polynomial 1-z0*z^(-1)"""
    assert z0 != 0
    return np.array([1, -z0])


def delay(d):
    """Return z^(-d)"""
    assert d >= 0
    z_d = np.zeros(d+1)
    z_d[-1] += 1
    return z_d


def calculate_rst(b_minus, a_minus, a_m, a0=np.array([1]), d=1, p=0):
    """Calculate the coefficient of the rst"""
    perturbation = P.polypow(zero(1), p)
    s2, r0 = solve_diophantine(P.polymul(perturbation, a_minus),
                               P.polymul(delay(1), b_minus), a_m)
    t0 = a0 * sum(a_m) / sum(b_minus)
    return r0, s2, t0


def solve_diophantine(a, b, c):
    """Solve diophantine equation a*x+b*y=c

    a, b, c: ndarray
    """
    na = a.shape[0]
    nb = b.shape[0]
    nc = c.shape[0]
    nx = nb - 1
    ny = na - 1
    n = na + nb - 2

    if nc < n:
        c = c.copy()
        c.resize(n)

    if nc <= n:
        nx = nb - 1
        ny = na - 1
    else:
        n = nc
        nx = nc - na + 1
        ny = na - 1
        print(nx, ny)

    s = np.zeros((n, n))

    for i in range(nx):
        s[i:na+i, i] = a

    for i in range(ny):
        s[i:nb+i, i+nx] = b

    res = np.linalg.solve(s, c)

    x = res[:nx]
    y = res[nx:]

    return x, y


class EasyRst(QWidget):
    def __init__(self):
        super().__init__()

        self.initControl()
        self.initUI()

    def initControl(self):
        self.Ts = 0.01
        self.k = 3.456/24.76
        self.tau = 1/24.76
        self.r = cnt.tf([293.9184, -229.4621], [1, 0])
        self.s = cnt.tf([1, -0.4469], [1, 0])
        self.t = cnt.tf([293.9184, -229.4621], [1, 0])
        self.time = np.linspace(0, 0.99, 100)

        self.gd = cnt.tf(self.k, [self.tau, 1, 0]).sample(self.Ts)

        # print(self.gd.num[0][0].shape, self.gd.den[0][0])
        # print(self.k, self.gd.zero(), self.gd.pole())

    def initUI(self):
        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        self.toolbar = NavigationToolbar(self.canvas, self)

        self.button = QPushButton('Plot')
        self.button.clicked.connect(self.plot)

        displayLayout = QVBoxLayout()
        displayLayout.addWidget(self.toolbar)
        displayLayout.addWidget(self.canvas)
        displayLayout.addWidget(self.button)

        self.setLayout(displayLayout)

    def computeResponse(self):
        y = self.t*cnt.feedback(self.gd/self.s, self.r)
        u = self.t*cnt.feedback(1/self.s, self.gd*self.r)

        _, self.stepY = cnt.step_response(y, self.time)
        _, self.stepU = cnt.step_response(u, self.time)

    def plot(self):
        self.figure.clear()

        self.computeResponse()

        axY = self.figure.add_subplot(121)
        axY.plot(self.time, self.stepY[0], label='Y')
        axY.set_title('Y response')
        axY.legend()

        axU = self.figure.add_subplot(122)
        axU.plot(self.time, self.stepU[0], label='U')
        axU.set_title('U response')
        axU.legend()

        self.canvas.draw()


if __name__ == '__main__':
    b_minus = 0.0001594*zero(-0.921)
    a_minus = zero(1)
    a_m = P.polypow(zero(0.7), 2)
    print(b_minus, a_minus, a_m)
    print(calculate_rst(b_minus, a_minus, a_m))
    # app = QApplication(sys.argv)
    # er = EasyRst()
    # er.show()
    # sys.exit(app.exec_())
