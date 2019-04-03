#!/usr/bin/env python3
import serial
import struct
import time
import numpy as np
import matplotlib.pyplot as plt

typesDict = {'char': 'c', 'bool': '?',
             'int8': 'b', 'uint8': 'B',
             'int16': 'h', 'uint16': 'H',
             'int32': 'i', 'uint32': 'I',
             'int64': 'l', 'uint64': 'L',
             'float': 'f'}


def computeFormat(structFormat):
    """Compute the format string for struct.(pack/unpack)"""
    structTypes = '='

    for t in structFormat:
        structTypes += typesDict[t]

    return structTypes


def readData(ser, structFormat):
    structTypes = computeFormat(structFormat)
    nbBytes = struct.calcsize(structTypes)
    # Wait until all the data is in the buffer
    while ser.in_waiting < nbBytes:
        pass
    # Read the raw data
    rawData = bytearray(nbBytes)
    ser.readinto(rawData)
    # Convert the raw data
    data = list(struct.unpack(structTypes, rawData))
    return data


def writeData(ser, structFormat, data):
    structTypes = computeFormat(structFormat)
    rawData = struct.pack(structTypes, *data)
    ser.write(rawData)


if __name__ == '__main__':
    portName = '/dev/ttyACM0'
    baudRate = 115200

    nbMeasure = 5
    nbSample = 100
    waitTime = 1000
    pwm = 220
    Ts = 0.005

    # Define the format of the structure of data sent
    structFormatConfig = ['uint8', 'uint16', 'uint16', 'uint8']
    structFormatMeasure = ['uint32', 'uint32', 'float']

    timestamps = np.zeros((nbMeasure, nbSample))
    positions = np.zeros((nbMeasure, nbSample))
    speeds = np.zeros((nbMeasure, nbSample))

    with serial.Serial(portName, baudRate, timeout=1) as ser:
        # Wait for the arduino to initilize
        time.sleep(2)
        # Write some data to the arduino
        writeData(ser, structFormatConfig, [nbMeasure, nbSample, waitTime, pwm])
        print(readData(ser, structFormatConfig))
        for i in range(nbMeasure):
            print(i, '/', nbMeasure)
            for j in range(nbSample):
                timestamps[i, j], positions[i, j], speeds[i, j] = readData(ser, structFormatMeasure)
        print(nbMeasure, '/', nbMeasure)

    speed = np.mean(speeds, axis=0)
    t = [i*Ts for i in range(len(speed))]

    plt.plot(t, speed)
    plt.xlabel("Time in seconds")
    plt.ylabel("Speed in steps/seconds")
    plt.show()
    np.savetxt('speed1.csv', speed)
