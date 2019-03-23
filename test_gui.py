#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QVBoxLayout, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import random
import matplotlib

matplotlib.use("Qt5Agg")


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

        layout = QVBoxLayout()
        layout.addWidget(self.toolbar)
        layout.addWidget(self.canvas)
        layout.addWidget(self.button)

        self.setLayout(layout)

    def plot(self):
        data1 = [random.random() for i in range(10)]
        data2 = [random.random() for i in range(10)]

        self.figure.clear()

        ax1 = self.figure.add_subplot(121)
        ax1.set_title('Plot 1')
        ax1.plot(data1, label='1')
        ax1.legend()

        ax2 = self.figure.add_subplot(122)
        ax2.set_title('Plot 2')
        ax2.plot(data2, label='2')
        ax2.legend()

        self.canvas.draw()


if __name__ == '__main__':

    app = QApplication(sys.argv)
    pt = PidTuning()
    pt.show()
    sys.exit(app.exec_())
