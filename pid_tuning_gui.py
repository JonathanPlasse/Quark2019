#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QFileDialog, QPushButton, QDoubleSpinBox, QVBoxLayout, QHBoxLayout, QFormLayout, QGroupBox, QApplication)
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

        self.initControl()
        self.initUI()

    def initControl(self):
        self.nbStep = 1633
        self.pwm = 220
        self.Ts = 0.005
        self.kp = 0.034467
        self.ki = 0.35558
        self.fileName = QFileDialog.getOpenFileName(self, 'Choose speed csv file', '.', 'CSV files (*.csv)')[0]
        self.speed = np.loadtxt(self.fileName)

        p0 = np.ones(2)

        self.t = np.array([self.Ts * i for i in range(len(self.speed))])

        p, _ = leastsq(residual, p0, args=(self.t, self.speed))
        self.k, self.tau = p

    def setKp(self, newKp):
        self.kp = newKp

    def setKi(self, newKi):
        self.ki = newKi

    def initUI(self):
        self.pSpinBox = QDoubleSpinBox()
        self.pSpinBox.setMaximum(10000)
        self.pSpinBox.setValue(self.kp)
        self.pSpinBox.valueChanged.connect(self.setKp)

        self.iSpinBox = QDoubleSpinBox()
        self.iSpinBox.setMaximum(10000)
        self.iSpinBox.setValue(self.ki)
        self.iSpinBox.valueChanged.connect(self.setKi)

        controlLayout = QFormLayout()
        controlLayout.addRow('P', self.pSpinBox)
        controlLayout.addRow('I', self.iSpinBox)

        controlGroupBox = QGroupBox('Control parameters')
        controlGroupBox.setLayout(controlLayout)

        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        self.toolbar = NavigationToolbar(self.canvas, self)

        self.button = QPushButton('Plot')
        self.button.clicked.connect(self.plot)

        displayLayout = QVBoxLayout()
        displayLayout.addWidget(self.toolbar)
        displayLayout.addWidget(self.canvas)
        displayLayout.addWidget(self.button)

        mainLayout = QHBoxLayout()
        mainLayout.addWidget(controlGroupBox)
        mainLayout.addLayout(displayLayout)

        self.setLayout(mainLayout)

    def computeResponse(self):
        c = cnt.tf([self.kp, self.ki*self.Ts-self.kp], [1, -1], self.Ts)

        gd = cnt.tf(self.k / self.pwm, [self.tau, 1]).sample(self.Ts)

        y = cnt.feedback(c*gd)
        u = cnt.feedback(c, gd)

        _, self.stepY = cnt.step_response(y, self.t)
        _, self.stepU = cnt.step_response(u, self.t)

    def plot(self):
        self.figure.clear()

        self.computeResponse()

        olAx = self.figure.add_subplot(131)
        olAx.plot(self.t, self.speed, label='real speed')
        olAx.plot(self.t, f(self.t, self.k, self.tau), label='model speed')
        olAx.set_title('Open-loop')
        olAx.legend()

        yAx = self.figure.add_subplot(132)
        yAx.plot(self.t, self.stepY[0], label='Y')
        yAx.set_title('Y response')
        yAx.legend()

        uAx = self.figure.add_subplot(133)
        uAx.plot(self.t, self.stepU[0], label='U')
        uAx.set_title('U response')
        uAx.legend()

        self.canvas.draw()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    pt = PidTuning()
    pt.show()
    sys.exit(app.exec_())
