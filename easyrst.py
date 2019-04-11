#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QVBoxLayout, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg\
                                        as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT\
                                        as NavigationToolbar
from matplotlib.figure import Figure
import matplotlib
import control as cnt
import numpy as np

matplotlib.use('Qt5Agg')


def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))


def residual(p, t, speed):
    return speed - f(t, *p)


def solve_diophantine(a, b, c):
    """Solve diophantine equation a*x+b*y=c"""
    na = a.shape[0]
    nb = b.shape[0]
    nx = nb - 1
    ny = na - 1
    n = na + nb - 2

    A = np.zeros((n, n))

    for i in range(nx):
        A[i:na+i, i] = a

    for i in range(ny):
        A[i:nb+i, i+nx] = b

    print(A)

    B = c
    res = np.linalg.solve(A, B)
    print(res)
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
        gd = cnt.tf(self.k, [self.tau, 1, 0]).sample(self.Ts)
        print(gd.num[0][0].shape, gd.den[0][0])

        y = self.t*cnt.feedback(gd/self.s, self.r)

        _, self.stepY = cnt.step_response(y, self.time)

    def plot(self):
        self.figure.clear()

        self.computeResponse()

        ax = self.figure.add_subplot(111)
        ax.plot(self.time, self.stepY[0], label='Y')
        ax.set_title('Y response')
        ax.legend()

        self.canvas.draw()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    er = EasyRst()
    er.show()
    sys.exit(app.exec_())
