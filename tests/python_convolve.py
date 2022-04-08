import random

import numpy as np
from scipy import io as spio
from scipy import signal


h_size = random.choice([3, 5, 7, 9])
h = np.random.uniform(0,100,size=(h_size, h_size))
x = np.random.uniform(0,100,size=np.random.randint(30,40,size=2))

y = signal.convolve2d(x, h, "same")

spio.mmwrite("x_convolve",x)
spio.mmwrite("y_convolve",y)
spio.mmwrite("h_convolve",h)

