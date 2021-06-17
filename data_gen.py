import time
import board
from adafruit_seesaw.seesaw import Seesaw

import csv
import random
import time

x_value = 0
total_1 = 1000
#total_2 = 1000

fieldnames = ["x_value", "total_1", "total_2"]



i2c_bus = board.I2C()

ss = Seesaw(i2c_bus, addr=0x36)
min = 2000
max = 0


with open('data.csv', 'w') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
    csv_writer.writeheader()

while True:

    with open('data.csv', 'a') as csv_file:
        csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

        info = {
            "x_value": x_value,
            "total_1": total_1,
           # "total_2": total_2
        }

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

        csv_writer.writerow(info)
        #print(x_value, total_1, total_2)
        print(x_value, total_1)

        x_value += 1
        #total_1 = total_1 + random.randint(-6, 8)
        total_1 = touch
        #total_2 = total_2 + random.randint(-5, 6)


    time.sleep(10)