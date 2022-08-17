import math
import pandas as pd


class Plot_Visualizer():
    def __init__(self):
        self.xvals = []
        self.yvals = []

    def read_csv_vals(self, filename):
        self.xvals.clear()
        self.yvals.clear()
        pd_handler = pd.read_csv(filename)
        # Remove null values
        pd_handler = pd_handler.dropna()
        self.xvals = list(pd_handler["Time"])
        self.yvals = list(pd_handler["Torque"])
        print(pd_handler.to_string())

    def get_xvals(self):
        return self.xvals

    def get_yvals(self):
        return self.yvals

    def print_vals(self):
        print(f"xvals: {self.xvals} \nyvals: {self.yvals}")

#     def plot_V():
#     global xvals, yvals
#     xvals.clear()
#     yvals.clear()
#     for i in range(-10, 10):
#         xvals.append(i)
#         yvals.append(-4*math.cos(i)+100)


# def plot_I():
#     global xvals, yvals
#     xvals.clear()
#     yvals.clear()
#     for i in range(-10, 10):
#         xvals.append(i)
#         yvals.append(4*math.tan(i)+100)


# def plot_P():
#     global xvals, yvals
#     xvals.clear()
#     yvals.clear()
#     for i in range(-10, 10):
#         xvals.append(i)
#         yvals.append(-(math.exp(i))+100)


# def plot_T():
#     global xvals, yvals
#     xvals.clear()
#     yvals.clear()
#     for i in range(-10, 10):
#         xvals.append(i)
#         yvals.append(math.exp(i)+100)
