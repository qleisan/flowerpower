# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time

import board

from adafruit_seesaw.seesaw import Seesaw



i2c_bus = board.I2C()

ss = Seesaw(i2c_bus, addr=0x36)
min = 2000
max = 0

while True:
    # read moisture level through capacitive touch pad
    touch = ss.moisture_read()
    if touch < min:
        min = touch
    if touch > max:
        max = touch

    # read temperature from the temperature sensor
    #temp = ss.get_temp()
    temp = 0


    print("min: " + str(min) + " max: " + str(max) + "  moisture: " + str(touch))
    time.sleep(1)
