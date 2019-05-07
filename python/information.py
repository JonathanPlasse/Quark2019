#!/usr/bin/python3
# -*- coding: utf-8 -*-

from binserial import BinSerial

if __name__ == "__main__":
    bser = BinSerial("/dev/ttyACM0", 115200)
    while True:
        print(bser.read(['float']*5))
