import math
import pandas as pd


class Plot_Visualizer():
    def __init__(self):
        self.last_plot_name = ""
        self.xvals = []
        self.torque_vals = []
        self.brake_vals = []
        self.current_vals = []
        self.voltage_vals = []
        self.power_vals = []
        self.pf_handler = None

    def get_last_plot_name(self):
        return self.last_plot_name

    def read_csv_vals(self, filename):
        self.xvals.clear()
        self.torque_vals.clear()
        self.brake_vals.clear()
        self.current_vals.clear()
        self.voltage_vals.clear()
        self.power_vals.clear()
        self.pd_handler = pd.read_csv(filename)
        # Remove null values
        self.pd_handler = self.pd_handler.dropna()
        # If there isnìt a Power column it can be created
        if 'Power' not in self.pd_handler.columns:
            self.pd_handler["Power"] = self.pd_handler["Voltage"] * \
                self.pd_handler["Current"]

        self.xvals = list(self.pd_handler["Time"])
        self.torque_vals = list(self.pd_handler["Torque"])
        self.brake_vals = list(self.pd_handler["Brake"])
        self.current_vals = list(self.pd_handler["Current"])
        self.voltage_vals = list(self.pd_handler["Voltage"])
        self.power_vals = list(self.pd_handler["Power"])
        # print(pd_handler.to_string())

    def get_csv_head(self):
        if not self.pd_handler.empty:
            return self.pd_handler.head(5)
        else:
            return False

    def get_csv_tail(self):
        if not self.pd_handler.empty:
            return self.pd_handler.tail(5)
        else:
            return False

    def get_xvals(self):
        return self.xvals

    def get_torque_vals(self):
        return self.torque_vals

    def get_brake_vals(self):
        return self.brake_vals

    def get_voltage_vals(self):
        return self.voltage_vals

    def get_current_vals(self):
        return self.current_vals

    def get_power_vals(self):
        return self.power_vals

    # def print_vals(self):
    #     print(f"xvals: {self.xvals} \nyvals: {self.yvals}")

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
