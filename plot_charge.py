#%%
import json
import re
from pprint import pprint 
import matplotlib.pyplot as plt
import numpy as np
import sys
import datetime

def on_pick(event):
    # On the pick event, find the original line corresponding to the legend
    # proxy line, and toggle its visibility.
    legline = event.artist
    origline = lined[legline]
    visible = not origline.get_visible()
    origline.set_visible(visible)
    # Change the alpha on the line in the legend so we can see what lines
    # have been toggled.
    legline.set_alpha(1.0 if visible else 0.2)
    fig.canvas.draw()


def create3listsbatterydatafromfile(filename):
    file1 = open(filename,"r")
    # builds json sting from file content that is parsed into a list
    content = file1.read()
    content_list = content.splitlines()
    json_str = '{"table":['
    for line in content_list:
        result = re.search('({.*})', line)
        mystr = result.group(1)
        json_str += mystr +","
    json_str = json_str[:-1]
    json_str += "]}"
    content_dict = json.loads(json_str)
    #pprint(content_dict)
    # for element in content_dict["table"]:
    #     print(element["counter"])

    table = content_dict["table"]
    counter_list = [d['counter'] for d in table]
    epoch_list = [int(d['time']) for d in table]
    batt1_list = [d['batt1'] for d in table]
    batt2_list = [d['batt2'] for d in table]
    batt3_list = [d['batt3'] for d in table]

    datetime_time = datetime.datetime.fromtimestamp(epoch_list[2]) # first sample (happened?) to be non-epoch
    print("start time: ", end='')
    print(datetime_time)
    datetime_time = datetime.datetime.fromtimestamp(epoch_list[-1])
    print("end time: ", end='')
    print(datetime_time)
    return counter_list, batt1_list, batt2_list, batt3_list


print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
if len(sys.argv) == 1:
    print("Provide at least one and max three argument(s) filename1, filename2, offset")
    exit(1)

# filename = "temp.json" 
filename = str(sys.argv[1])
#%%
mytuple = create3listsbatterydatafromfile(filename)
counter_list = mytuple[0]
batt1_list = mytuple[1]
batt2_list = mytuple[2]
batt3_list = mytuple[3]
#%%
#plt.cla()
#fig, ax = plt.figure(figsize=(17, 8))
fig, ax = plt.subplots()
fig.canvas.mpl_connect('pick_event', on_pick)
line1, = plt.plot(counter_list, batt1_list, label='Channel 1')
line2, = plt.plot(counter_list, batt2_list, label='Channel 2')
line3, = plt.plot(counter_list, batt3_list, label='Channel 3')

if len(sys.argv) > 2:
    filename = str(sys.argv[2])
    mytuple = create3listsbatterydatafromfile(filename)
    counter2_list = mytuple[0]
    if len(sys.argv) > 3:
        # offsetvalue provided by user
        counter2_list = [x+int(sys.argv[3]) for x in counter2_list]
    batt4_list = mytuple[1]
    batt5_list = mytuple[2]
    batt6_list = mytuple[3]
    #%%
    #plt.cla()
    #fig, ax = plt.figure(figsize=(17, 8))
    # fig, ax = plt.subplots()
    # fig.canvas.mpl_connect('pick_event', on_pick)
    line4, = plt.plot(counter2_list, batt4_list, label='Channel 4')
    line5, = plt.plot(counter2_list, batt5_list, label='Channel 5')
    line6, = plt.plot(counter2_list, batt6_list, label='Channel 6')
    lines = [line1, line2, line3, line4, line5, line6]
else:
    lines = [line1, line2, line3]

leg = ax.legend(fancybox=True, shadow=True)
lined = {}  # Will map legend lines to original lines.
for legline, origline in zip(leg.get_lines(), lines):
    legline.set_picker(True)  # Enable picking on the legend line.
    lined[legline] = origline

#plt.legend(loc='upper right')
#plt.tight_layout()
plt.show()
