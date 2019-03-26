#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QDoubleSpinBox, QSpinBox, QLineEdit, QVBoxLayout, QHBoxLayout, QFormLayout, QGroupBox, QApplication)
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

class MotorModel(QWidget):
    def __init__(self):
        super().__init__()

        self.initParameters()
        self.initUI()

    def initParameters(self):
        self.pwm = 220
        self.ts = 0.005
        self.nbMeasure = 10
        self.nbSample = 100
        self.waitTime = 1000
        self.fileName = 'speed1.csv'

    def setPwm(self, newPwm):
        self.pwm = newPwm

    def setTs(self, newTs):
        self.ts = newTs

    def setNbMeasure(self, newNbMeasure):
        self.nbMeasure = newNbMeasure

    def setNbSample(self, newNbSample):
        self.nbSample = newNbSample

    def setWaitTime(self, newWaitTime):
        self.waitTime = newWaitTime

    def setFileName(self, newFileName):
        self.fileName = newFileName

    def initUI(self):

        # Parameters
        self.pwmSpinBox = QSpinBox()
        self.pwmSpinBox.setMinimum(0)
        self.pwmSpinBox.setMaximum(255)
        self.pwmSpinBox.setValue(self.pwm)
        self.pwmSpinBox.valueChanged.connect(self.setPwm)

        self.tsSpinBox = QDoubleSpinBox()
        self.tsSpinBox.setMinimum(0)
        self.tsSpinBox.setMaximum(1)
        self.tsSpinBox.setSingleStep(0.001)
        self.tsSpinBox.setValue(self.ts)
        self.tsSpinBox.valueChanged.connect(self.setTs)

        self.nbMeasureSpinBox = QSpinBox()
        self.nbMeasureSpinBox.setMinimum(1)
        self.nbMeasureSpinBox.setMaximum(100)
        self.nbMeasureSpinBox.setValue(self.nbMeasure)
        self.nbMeasureSpinBox.valueChanged.connect(self.setNbMeasure)

        self.nbSampleSpinBox = QSpinBox()
        self.nbSampleSpinBox.setMinimum(10)
        self.nbSampleSpinBox.setMaximum(1000)
        self.nbSampleSpinBox.setValue(self.nbSample)
        self.nbSampleSpinBox.valueChanged.connect(self.setNbSample)

        self.waitTimeSpinBox = QSpinBox()
        self.waitTimeSpinBox.setMinimum(100)
        self.waitTimeSpinBox.setMaximum(10000)
        self.waitTimeSpinBox.setValue(self.waitTime)
        self.waitTimeSpinBox.valueChanged.connect(self.setWaitTime)

        self.fileNameLineEdit = QLineEdit()
        self.fileNameLineEdit.setText(self.fileName)
        self.fileNameLineEdit.textChanged.connect(self.setFileName)

        parametersLayout = QFormLayout()
        parametersLayout.addRow('PWM', self.pwmSpinBox)
        parametersLayout.addRow('Sample Time', self.tsSpinBox)
        parametersLayout.addRow('Number of Measure', self.nbMeasureSpinBox)
        parametersLayout.addRow('Number of Sample', self.nbSampleSpinBox)
        parametersLayout.addRow('Wait Time', self.waitTimeSpinBox)
        parametersLayout.addRow('File Name', self.fileNameLineEdit)

        parametersGroupBox = QGroupBox('Measure parameters')
        parametersGroupBox.setLayout(parametersLayout)

        # Display
        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        self.toolbar = NavigationToolbar(self.canvas, self)

        self.button = QPushButton('Run')
        self.button.clicked.connect(self.runStepResponse)

        displayLayout = QVBoxLayout()
        displayLayout.addWidget(self.toolbar)
        displayLayout.addWidget(self.canvas)
        displayLayout.addWidget(self.button)

        # Main
        mainLayout = QHBoxLayout()
        mainLayout.addWidget(parametersGroupBox)
        mainLayout.addLayout(displayLayout)

        self.setLayout(mainLayout)

    def runStepResponse(self):
        self.speed = np.loadtxt(self.fileName)
        self.computeRegression()
        self.plot()

    def computeRegression(self):
        p0 = np.ones(2)

        self.t = np.arange(0, self.ts * self.nbSample, self.ts)

        p, _ = leastsq(residual, p0, args=(self.t, self.speed))
        self.k, self.tau = p

    def plot(self):
        self.figure.clear()

        ax = self.figure.add_subplot(111)
        ax.plot(self.t, self.speed, label='real speed')
        ax.plot(self.t, f(self.t, self.k, self.tau), label='model speed')
        ax.set_title('Motor Step Response')
        ax.legend()

        self.canvas.draw()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mm = MotorModel()
    mm.show()
    sys.exit(app.exec_())
