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
        initial_time = self.pd_handler.iloc[0]['Time [ms]']
        # if time is relative then convert to absolute
        if initial_time != 0:
            self.pd_handler['Time [ms]'] -= self.pd_handler.iloc[0]['Time [ms]']
        # If there isnìt a Power column it can be created
        if 'Power [W]' not in self.pd_handler.columns:
            self.pd_handler["Power [W]"] = self.pd_handler["Voltage [V]"] * \
                self.pd_handler["Current [A]"]

        self.xvals = list(self.pd_handler["Time [ms]"])
        self.torque_vals = list(self.pd_handler["Torque [Nm]"])
        self.brake_vals = list(self.pd_handler["Brake [%]"])
        self.current_vals = list(self.pd_handler["Current [A]"])
        self.voltage_vals = list(self.pd_handler["Voltage [V]"])
        self.power_vals = list(self.pd_handler["Power [W]"])
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

    def get_torque_max(self):
        return self.pd_handler['Torque [Nm]'].max()

    def get_torque_min(self):
        return self.pd_handler['Torque [Nm]'].min()

    def get_voltage_max(self):
        return self.pd_handler['Voltage [V]'].max()

    def get_voltage_min(self):
        return self.pd_handler['Voltage [V]'].min()

    def get_current_max(self):
        return self.pd_handler['Current [A]'].max()

    def get_current_min(self):
        return self.pd_handler['Current [A]'].min()

    def get_power_max(self):
        return self.pd_handler['Power [W]'].max()

    def get_power_min(self):
        return self.pd_handler['Power [W]'].min()

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
