import random

import numpy as np
from scipy import io as spio
from scipy import signal

def normalize(im, im_min=0, im_max=255):
    
    im[im>im_max] = im_max
    im[im<im_min] = im_min

    return im


if __name__ == "__main__":
    x = 400*(np.random.random(size=(100,100))-0.2)
    y = normalize(x)

    spio.mmwrite("x_normalize",x)
    spio.mmwrite("y_normalize",y)
