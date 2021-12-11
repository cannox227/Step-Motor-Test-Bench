import math

xvals = []
yvals = []

def get_xvals():
    return xvals

def get_yvals():
    return yvals

def print_vals():
    print(f"xvals: {xvals} \nyvals: {yvals}")
def plot_V():
    global xvals, yvals
    xvals.clear()
    yvals.clear()
    for i in range(-10,10):
        xvals.append(i)
        yvals.append(-4*math.cos(i)+100)

def plot_I():
    global xvals, yvals
    xvals.clear()
    yvals.clear()
    for i in range(-10,10):
        xvals.append(i)
        yvals.append(4*math.tan(i)+100)

def plot_P():
    global xvals, yvals
    xvals.clear()
    yvals.clear()
    for i in range(-10,10):
        xvals.append(i)
        yvals.append(-(math.exp(i))+100)

def plot_T():
    global xvals, yvals
    xvals.clear()
    yvals.clear()
    for i in range(-10,10):
        xvals.append(i)
        yvals.append(math.exp(i)+100)

