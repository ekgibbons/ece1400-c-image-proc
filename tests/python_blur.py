import numpy as np
from scipy import signal
from scipy import io as spio

import python_normalize

def blur(im):
    gaussian = 1/273*np.array([[1, 4, 7, 4, 1],
                               [4, 16, 26, 16, 4],
                               [7, 26, 41, 26, 7],
                               [4, 16, 26, 16, 4],
                               [1, 4, 7, 4, 1]])

    im_blur = signal.convolve2d(im, gaussian,"same")
    im_blur = python_normalize.normalize(im_blur)
    
    return im_blur


if __name__ == "__main__":
    x = np.random.randint(0,256,size=(200,200))
    
    y = blur(x)

    spio.mmwrite("x_blur",x)
    spio.mmwrite("y_blur",y)
