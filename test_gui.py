#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QLabel, QSpinBox, QVBoxLayout, QHBoxLayout, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import matplotlib
from scipy.optimize import leastsq
import control as cnt
import numpy as np

matplotlib.use('Qt5Agg')

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

class PidTuning(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

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

        self.pLabel = QLabel('P')
        self.pSpinBox = QSpinBox()
        self.pSpinBox.setMaximum(10000)

        pLayout = QHBoxLayout()
        pLayout.addWidget(self.pLabel)
        pLayout.addWidget(self.pSpinBox)

        self.iLabel = QLabel('I')
        self.iSpinBox = QSpinBox()
        self.iSpinBox.setMaximum(10000)

        iLayout = QHBoxLayout()
        iLayout.addWidget(self.iLabel)
        iLayout.addWidget(self.iSpinBox)

        controlLayout = QVBoxLayout()
        controlLayout.addStretch(1)
        controlLayout.addLayout(pLayout)
        controlLayout.addLayout(iLayout)
        controlLayout.addStretch(2)

        mainLayout = QHBoxLayout()
        mainLayout.addLayout(controlLayout)
        mainLayout.addLayout(displayLayout)

        self.setLayout(mainLayout)

    def plot(self):
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

        self.figure.clear()

        olAx = self.figure.add_subplot(131)
        olAx.plot(t, speed, label='real speed')
        olAx.plot(t, f(t, *p), label='model speed')
        olAx.set_title('Open-loop')
        olAx.legend()

        yAx = self.figure.add_subplot(132)
        yAx.plot(t, stepY[0], label='Y')
        yAx.set_title('Y response')
        yAx.legend()

        uAx = self.figure.add_subplot(133)
        uAx.plot(t, stepU[0], label='U')
        uAx.set_title('U response')
        uAx.legend()

        self.canvas.draw()


if __name__ == '__main__':

    app = QApplication(sys.argv)
    pt = PidTuning()
    pt.show()
    sys.exit(app.exec_())
