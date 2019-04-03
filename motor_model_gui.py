#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import (QWidget, QPushButton, QDoubleSpinBox, QSpinBox,
                             QLineEdit, QVBoxLayout, QHBoxLayout, QFormLayout,
                             QGroupBox, QApplication)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg\
                                        as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT\
                                        as NavigationToolbar
from matplotlib.figure import Figure
import matplotlib
from scipy.optimize import leastsq
import numpy as np
import yaml
from binserial import BinSerial

matplotlib.use('Qt5Agg')


def f(t, k, tau):
    return k * (1 - np.exp(-t / tau))


def residual(p, t, speed):
    return speed - f(t, *p)


class MotorModel(QWidget):
    def __init__(self):
        super().__init__()
        self.init_parameters()
        self.init_ui()

    def init_parameters(self):
        with open('config.yml', 'r') as f:
            config = yaml.load(f.read())
        self.port_name = config['port_name']
        self.baud_rate = config['baud_rate']
        self.pwm = config['pwm']
        self.ts = config['ts']
        self.nb_measure = config['nb_measure']
        self.nb_sample = config['nb_sample']
        self.wait_time = config['wait_time']
        self.file_name = config['file_name']

    def set_pwm(self, new_pwm):
        self.pwm = new_pwm

    def set_ts(self, new_ts):
        self.ts = new_ts

    def set_nb_measure(self, new_nb_measure):
        self.nb_measure = new_nb_measure

    def set_nb_sample(self, new_nb_sample):
        self.nb_sample = new_nb_sample

    def set_wait_time(self, new_wait_time):
        self.wait_time = new_wait_time

    def set_file_name(self, new_file_name):
        self.file_name = new_file_name

    def init_ui(self):

        # Parameters
        self.pwm_spin = QSpinBox()
        self.pwm_spin.setMinimum(0)
        self.pwm_spin.setMaximum(255)
        self.pwm_spin.setValue(self.pwm)
        self.pwm_spin.valueChanged.connect(self.set_pwm)

        self.ts_spin = QDoubleSpinBox()
        self.ts_spin.setMinimum(0)
        self.ts_spin.setMaximum(1)
        self.ts_spin.setSingleStep(0.001)
        self.ts_spin.setDecimals(3)
        self.ts_spin.setValue(self.ts)
        self.ts_spin.valueChanged.connect(self.set_ts)

        self.nb_measure_spin = QSpinBox()
        self.nb_measure_spin.setMinimum(1)
        self.nb_measure_spin.setMaximum(100)
        self.nb_measure_spin.setValue(self.nb_measure)
        self.nb_measure_spin.valueChanged.connect(self.set_nb_measure)

        self.nb_sample_spin = QSpinBox()
        self.nb_sample_spin.setMinimum(10)
        self.nb_sample_spin.setMaximum(1000)
        self.nb_sample_spin.setValue(self.nb_sample)
        self.nb_sample_spin.valueChanged.connect(self.set_nb_sample)

        self.wait_time_spin = QSpinBox()
        self.wait_time_spin.setMinimum(100)
        self.wait_time_spin.setMaximum(10000)
        self.wait_time_spin.setValue(self.wait_time)
        self.wait_time_spin.valueChanged.connect(self.set_wait_time)

        self.file_name_edit = QLineEdit()
        self.file_name_edit.setText(self.file_name)
        self.file_name_edit.textChanged.connect(self.set_file_name)

        parameters_layout = QFormLayout()
        parameters_layout.addRow('PWM', self.pwm_spin)
        parameters_layout.addRow('Sample Time', self.ts_spin)
        parameters_layout.addRow('Number of Measure', self.nb_measure_spin)
        parameters_layout.addRow('Number of Sample', self.nb_sample_spin)
        parameters_layout.addRow('Wait Time', self.wait_time_spin)
        parameters_layout.addRow('File Name', self.file_name_edit)

        parameters_group = QGroupBox('Measure parameters')
        parameters_group.setLayout(parameters_layout)

        # Display
        self.figure = Figure()
        self.canvas = FigureCanvas(self.figure)
        self.toolbar = NavigationToolbar(self.canvas, self)

        self.run_button = QPushButton('Run')
        self.run_button.clicked.connect(self.run_step_response)

        display_layout = QVBoxLayout()
        display_layout.addWidget(self.toolbar)
        display_layout.addWidget(self.canvas)
        display_layout.addWidget(self.run_button)

        # Main
        main_layout = QHBoxLayout()
        main_layout.addWidget(parameters_group)
        main_layout.addLayout(display_layout)

        self.setLayout(main_layout)

    def get_step_response(self):
        """run the step response and get the measures"""
        bser = BinSerial(self.port_name, self.baud_rate)

        # Define the format of the structure of data sent
        structFormatConfig = ['uint8', 'uint16', 'uint16', 'uint8']
        structFormatMeasure = ['uint32', 'uint32', 'float']

        timestamps = np.zeros((self.nb_measure, self.nb_sample))
        positions = np.zeros((self.nb_measure, self.nb_sample))
        speeds = np.zeros((self.nb_measure, self.nb_sample))

        # Write some data to the arduino
        bser.write(structFormatConfig, [self.nb_measure, self.nb_sample,
                                        self.wait_time, self.pwm])
        print(bser.read(structFormatConfig))
        for i in range(self.nb_measure):
            print(i, '/', self.nb_measure)
            for j in range(self.nb_sample):
                timestamps[i, j], positions[i, j], speeds[i, j]\
                    = bser.read(structFormatMeasure)
        print(self.nb_measure, '/', self.nb_measure)

        self.speed = np.mean(speeds, axis=0)

    def run_step_response(self):
        self.get_step_response()
        # self.speed = np.loadtxt(self.file_name)
        self.compute_regression()
        self.plot()

    def compute_regression(self):
        p0 = np.ones(2)

        self.t = np.arange(0, self.ts * self.nb_sample, self.ts)

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
