import numpy as np


data = np.random.randint(0,256,size=(20*40*3))

fp = open("color.txt","w")
for data_point in data:
    fp.write("%.16f\n" % data_point)
fp.close()

data_bw = (0.299*data[0::3] +
              0.587*data[1::3] +
              0.114*data[2::3])

fp = open("bw.txt","w")
for data_point in data_bw:
    fp.write("%.16f\n" % data_point)
fp.close()

