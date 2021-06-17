import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from scipy import signal


plt.style.use('fivethirtyeight')

x_vals = []
y_vals = []

index = count()


def animate(i):
    data = pd.read_csv('data.csv')
    x = data['x_value']
    y1 = data['total_1']
    y2 = data['total_2']

    #TODO: vilseledande användning av "x"

    L=60 #L-point filter
    b = (np.ones(L))/L #numerator co-effs of filter transfer function
    a = np.ones(1)  #denominator co-effs of filter transfer function
    #x = np.random.randn(N) #10 random samples for x
    #x = np.ones(N)  #denominator co-effs of filter transfer function
    x = y1.to_numpy()
    print(len(x))
    #exit(0)
    #y = signal.convolve(x,b) #filter output using convolution

    y = signal.lfilter(b,a,x) #filter output using lfilter function

    plt.cla()
    i = list(range(0,len(x)))
    plt.plot(i, x, label='Channel 1')
    plt.plot(i, y, label='Channel 2')

    plt.legend(loc='upper left')
    plt.tight_layout()
    plt.show()

    # for t in y1.iteritems():
    #     print(t)

    print(type(y1))
    print(y1)
    apa = y1.to_numpy()
    print("----")
    print(apa)
    print(type(apa))

    # original -----------------

    # print(type(y1))
    # plt.cla()

    # plt.plot(x, y1, label='Channel 1')
    # plt.plot(x, y2, label='Channel 2')

    # plt.legend(loc='upper left')
    # plt.tight_layout()
    # plt.show()


# ani = FuncAnimation(plt.gcf(), animate, interval=1000)

# plt.tight_layout()
# plt.show()
animate(0)
# print(i, x,y)


print("hello friend")
