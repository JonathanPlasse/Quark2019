#!/usr/bin/env python3
import serial
import io
import time
import matplotlib.pyplot as plt

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

    speeds = []

    lastTime = time.time()
    while time.time()-lastTime < 2:
        speed = sio.readline().strip()
        if speed != '':
            lastTime = time.time()
            print(float(speed))
            speeds.append(speed)

    t = [i*0.005 for i in range(len(speeds))]
    plt.plot(t, speeds)
    plt.show()
