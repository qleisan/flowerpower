import json
import re
from pprint import pprint 
import matplotlib.pyplot as plt
import numpy as np
import sys
import datetime

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
# file1 = open("temp.json","r") 
file1 = open(str(sys.argv[1]), "r") 

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


#plt.cla()
plt.figure(figsize=(17, 8))
plt.plot(counter_list, batt1_list, label='Channel 1')
plt.plot(counter_list, batt2_list, label='Channel 2')
plt.plot(counter_list, batt3_list, label='Channel 3')

plt.legend(loc='upper right')
plt.tight_layout()
plt.show()
