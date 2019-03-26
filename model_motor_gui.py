#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QDoubleSpinBox, QSpinBox, QVBoxLayout, QHBoxLayout, QFormLayout, QGroupBox, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import matplotlib
from scipy.optimize import leastsq
import numpy as np

matplotlib.use('Qt5Agg')

def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))

def residual(p, t, speed):
    return speed - f(t, *p)

class ModelMotor(QWidget):
    def __init__(self):
        super().__init__()

        self.initParameters()
        self.initUI()

    def initParameters(self):
        self.pwm = 220
        self.ts = 0.005
        self.nbMeasure = 10
        self.nbSample = 200
        self.waitTime = 1000
        self.fileName = 'speed.csv'

    def setPwm(self, newPwm):
        self.pwm = newPwm

    def initUI(self):
        self.pwmSpinBox = QSpinBox()
        self.pwmSpinBox.setMinimum(0)
        self.pwmSpinBox.setMaximum(255)
        self.pwmSpinBox.setValue(self.pwm)
        self.pwmSpinBox.valueChanged.connect(self.setPwm)

        self.tsSpinBox = QDoubleSpinBox()
        self.tsSpinBox.setMinimum(0)
        self.tsSpinBox.setMaximum(1)
        self.tsSpinBox.setStep(1)
        self.tsSpinBox.setValue(self.pwm)
        self.tsSpinBox.valueChanged.connect(self.setPwm)

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

    def runStepResponse(self):
        pass

    def computeRegression(self):
        p0 = np.ones(2)

        self.t = np.array([self.Ts * i for i in range(self.nbSample)])

        p, _ = leastsq(residual, p0, args=(self.t, self.speed))
        self.k, self.tau = p

    def plot(self):
        self.figure.clear()

        self.computeResponse()

        ax = self.figure.add_subplot(111)
        ax.plot(self.t, self.speed, label='real speed')
        ax.plot(self.t, f(self.t, self.k, self.tau), label='model speed')
        ax.set_title('Motor Step Response')
        ax.legend()

        self.canvas.draw()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mm = ModelMotor()
    mm.show()
    sys.exit(app.exec_())
