import random

import numpy as np
from scipy import signal
from scipy import io as spio

import python_normalize
import python_blur


def sharpen(im, strength):
    
    laplacian = np.array([[0, -1, 0],
                          [-1, 4, -1],
                          [0, -1, 0]])
    
    im_blur = python_blur.blur(im)
    im_edge = signal.convolve2d(im_blur, laplacian, "same")

    im_sharp = im + strength*im_edge

    im_sharp = python_normalize.normalize(im_sharp)
    
    return im_sharp


if __name__ == "__main__":
    x = np.random.randint(0,256,size=(200,200))
    strength = random.uniform(0,5)

    y = sharpen(x, strength)

    fp = open("strength.txt","w")
    fp.write("%.16f" % strength)
    fp.close()

    spio.mmwrite("x_sharp",x)
    spio.mmwrite("y_sharp",y)
