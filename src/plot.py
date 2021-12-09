import math

xvals = []
yvals = []

def get_xvals():
    return xvals

def get_yvals():
    return yvals

for i in range(-10,10):
    xvals.append(i)
    yvals.append(math.exp(i)+100)

