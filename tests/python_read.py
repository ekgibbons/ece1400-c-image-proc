import numpy as np

import imageio

# im = np.random.randint(0,256,size=(100,200,3)).astype(np.uint8)

im = np.arange(24).reshape(2,4,3).astype(np.uint8)

imageio.imwrite("read_im.jpg",im)
