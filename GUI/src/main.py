from email.policy import default
from readline import append_history_file
from traceback import print_tb
import dearpygui.dearpygui as dpg
from matplotlib.style import available
import plot as plot
import motor_config
import micro_serial_handler
import sys
import os


light_blue = (0, 153, 255)


def devices_selector_callback(sender, app_data):
    print(app_data)


def show_settings(sender, app_data):
    if app_data == "Voltage mode":
        dpg.show_item("motor_settings_voltage_mode")
        dpg.hide_item("motor_settings_current_mode")
    elif app_data == "Current mode":
        dpg.show_item("motor_settings_current_mode")
        dpg.hide_item("motor_settings_voltage_mode")


# Graphs
dpg.create_context()
with dpg.window(label="Motor Voltage plot", tag="Motor Voltage plot", pos=(300, 0), show=False):
    with dpg.plot(label="Voltage - w", height=300, width=300):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="w")
        dpg.add_plot_axis(dpg.mvXAxis, label="V", tag="y-axis-V")
        dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        ), tag="V vals", label="V-w", parent="y-axis-V")

with dpg.window(label="Motor Torque plot", tag="Motor Torque plot", pos=(300, 150), show=False):
    with dpg.plot(label="Torque - w", height=300, width=300):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="w")
        dpg.add_plot_axis(dpg.mvXAxis, label="T", tag="y-axis-T")
        dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        ), tag="T vals", label="T-w", parent="y-axis-T")

with dpg.window(label="Motor Power plot", tag="Motor Power plot", pos=(300, 50), show=False):
    with dpg.plot(label="Power - w", height=300, width=300):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="w")
        dpg.add_plot_axis(dpg.mvXAxis, label="P", tag="y-axis-P")
        dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        ), tag="P vals", label="P-w", parent="y-axis-P")

with dpg.window(label="Motor Current plot", tag="Motor Current plot", pos=(300, 100), show=False):
    with dpg.plot(label="Current - w", height=300, width=300):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="w")
        dpg.add_plot_axis(dpg.mvXAxis, label="I", tag="y-axis-I")
        dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        ), tag="I vals", label="I-w", parent="y-axis-I")


# Connection error dialog
with dpg.window(label="Connection error", tag="connection_error_dialog", pos=(300, 300), show=False):
    dpg.add_text("Serial connection error", tag="Connection error")
    dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
        item="connection_error_dialog"))


def main():

    # import classes
    msc_handler = micro_serial_handler.Micro_serial_handler()
    motor_configuration = motor_config.Motor_Config()

    # import JSON config file
    motor_configuration.read_config_file("configs/config000.json")

    # File manager read
    with dpg.file_dialog(directory_selector=False, show=False, user_data=motor_configuration, callback=manage_file, tag="file_dialog_r", height=50):
        dpg.add_file_extension(".json")

    # File manager write
    with dpg.file_dialog(directory_selector=False, show=False, user_data=motor_configuration, callback=create_file, tag="file_dialog_c", height=50):
        dpg.add_file_extension(".json")

    # Serial window
    with dpg.window(label="Serial interface", tag="Serial interface", pos=(400, 0), width=300):
        dpg.add_button(label="Refresh device list",
                       callback=lambda: update_device_list(msc_handler))
        dpg.add_text("Available devices")
        dpg.add_combo(items=msc_handler.get_available_devices(),
                      tag="device_list", callback=devices_selector_callback)
        dpg.add_button(label="CONNECT",
                       callback=lambda: connect_to_device(msc_handler))
        dpg.add_text("Connection state: NOT CONNECTED",
                     tag="connection_state_text_field")
        dpg.add_button(label="Send hello",
                       callback=lambda: send_hello(msc_handler))
        with dpg.group() as cmd_input_section:
            dpg.add_text("Command input")
            dpg.add_input_text(tag="command_input")
            dpg.add_button(label="SEND COMMAND",
                           callback=lambda: send_command(msc_handler))

    # Motor settings window
    with dpg.window(label="Motor settings", tag="Motor settings"):

        with dpg.menu_bar():
            with dpg.menu(label="File"):
                dpg.add_menu_item(label="New config file",
                                  callback=lambda: dpg.show_item("file_dialog_c"))
                dpg.add_menu_item(label="Open config file",
                                  callback=lambda: dpg.show_item("file_dialog_r"))

            with dpg.menu(label="Edit"):
                dpg.add_menu_item(label="Copy")
                dpg.add_menu_item(label="Paste")
                dpg.add_menu_item(label="Cut")

        dpg.add_text(
            f"Motor configuration file used: {motor_configuration.get_value('config_name')}", tag="current_config_open")
        with dpg.group() as motor_settings:

            dpg.add_text("Common settings", color=light_blue)
            dpg.add_input_float(
                tag="max_speed", label="Max speed [step/s] (min: 15.25, max: 15610)", default_value=15.25, max_value=15610, min_value=15.25, min_clamped=True, max_clamped=True, width=100)
            dpg.add_input_float(
                tag="min_speed", label="Min speed [step/s] (min: 0, max: 976.3)", default_value=0, max_value=976.3, min_value=0, min_clamped=True, max_clamped=True, width=100)
            dpg.add_input_float(
                tag="full_step_speed", label="Full step speed [step/s] (min: 7.63, max: 15625)", default_value=7.63, max_value=15625, min_value=7.63, min_clamped=True, max_clamped=True, width=100)
            # check the enum
            dpg.add_input_float(tag="overcurrent_threshold",
                                label="Overcurrent threshold", default_value=281.25,  min_clamped=True, max_clamped=True, width=100)
            dpg.add_combo(tag="step_mode", label="Step mode",
                          items=["A", "B", "C"], width=100)
            dpg.add_separator()
            dpg.add_text("Operating mode", color=light_blue)
            dpg.add_radio_button(tag="working_mode", horizontal=True, items=[
                                 "Voltage mode", "Current mode"], default_value="Voltage mode", callback=show_settings)

            # Motor settings in current mode
            with dpg.group(xoffset=10, tag="motor_settings_current_mode", show=False) as motor_settings_current_mode:
                dpg.add_input_float(
                    tag="current_hold_torque_mv", label="Hold torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="current_running_torque_mv", label="Running torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="current_acceleration_torque_mv", label="Acceleration torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="current_deceleration_torque_mv", label="Deceleration torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)

                dpg.add_input_float(
                    tag="current_min_on_time_us", label="Minmum on-time [us] (min: 0.5, max: 64)", default_value=0.5, max_value=64, min_value=0.5, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="current_min_off_time_us", label="Minmum off-time [us] (min: 0.5, max: 64)", default_value=0.5, max_value=64, min_value=0.5, format="%.2f", min_clamped=True, max_clamped=True, width=100)

            # Motor settings in voltage mode
            with dpg.group(xoffset=10, tag="motor_settings_voltage_mode") as motor_settings_voltage_mode:
                dpg.add_input_float(tag="voltage_hold_perc", label="Hold DT [%] (min: 0, max: 99.6)", default_value=0, min_value=0,
                                    max_value=99.6, min_clamped=True, max_clamped=True, width=100, format="%.2f")
                dpg.add_input_float(tag="voltage_run_perc", label="Run DT [%] (min: 0, max: 99.6)", default_value=0, min_value=0,
                                    max_value=99.6, min_clamped=True, max_clamped=True, width=100, format="%.2f")
                dpg.add_input_float(tag="voltage_acc_perc", label="Acceleration DT [%] (min: 0, max: 99.6)", default_value=0, min_value=0,
                                    width=100, max_value=99.6,  min_clamped=True, max_clamped=True, format="%.2f")
                dpg.add_input_float(tag="voltage_dec_perc", label="Deceleration DT [%] (min: 0, max: 99.6)", default_value=0, min_value=0,
                                    max_value=99.6, width=100, min_clamped=True, max_clamped=True, format="%.2f")
                dpg.add_input_float(
                    tag="voltage_min_on_time_us", label="Minmum on-time [us] (min: 0.5, max: 64)", default_value=0.5, max_value=64, min_value=0.5, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="voltage_min_off_time_us", label="Minmum off-time [us] (min: 0.5, max: 64)", default_value=0.5, max_value=64, min_value=0.5, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="bemf_start_slope", label="BEMF start slope - BEMF compensation % [step/s] (min: 0, max: 0.4)", default_value=0, max_value=0.4, min_value=0, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="bemf_final_acc_slope", label="BEMF final acc slope - BEMF compensation % [step/s] (min: 0, max: 0.4)", default_value=0, max_value=0.4, min_value=0, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="bemf_final_dec_slope", label="BEMF final dec slope - BEMF compensation % [step/s] (min: 0, max: 0.4)", default_value=0, max_value=0.4, min_value=0, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="thermal_compensation", label="Thermal compensation (min: 1, max: 1.46875)", default_value=1, max_value=1.46875, min_value=1, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                dpg.add_input_float(
                    tag="stall_threshold", label="Stall threshold [mV] (min: 31.25, max: 1000)", default_value=31.25, max_value=1000, min_value=31.25, format="%.2f", min_clamped=True, max_clamped=True, width=100)

            dpg.add_text(label="")
            dpg.add_separator()
            dpg.add_text(label="")

            with dpg.group(horizontal=True) as plot_params_VT:
                dpg.add_button(label="Plot V-t", callback=lambda: plot_V())
                dpg.add_button(label="Plot T-t", callback=lambda: plot_T())
            with dpg.group(horizontal=True) as plot_params_PI:
                dpg.add_button(label="Plot P-t", callback=lambda: plot_P())
                dpg.add_button(label="Plot I-t", callback=lambda: plot_I())

            dpg.add_text(label="")
            dpg.add_separator()

            with dpg.group(horizontal=True) as bottom_buttons:
                dpg.add_button(label="Save with override",
                               callback=save_callback, user_data=motor_configuration)
                dpg.add_button(label="Exit", callback=exit_callback)

    # Create viewport where windows will be contained
    dpg.create_viewport(
        title='ProM Motor driver configurator', x_pos=0, y_pos=0)
    # Fill the viewport with a specifig window using its tag
    # dpg.set_primary_window("Motor settings", True)
    # Necessary setup phase
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()


def plot_V():
    plot.plot_V()
    dpg.set_value("V vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Voltage plot")


def plot_P():
    plot.plot_P()
    dpg.set_value("P vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Power plot")


def plot_I():
    plot.plot_I()
    dpg.set_value("I vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Current plot")


def plot_T():
    plot.plot_T()
    dpg.set_value("T vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Torque plot")


def update_device_list(msc):
    msc.update_available_devices()
    dpg.configure_item(
        "device_list", items=msc.get_available_devices_by_name())


def connect_to_device(msc):
    try:
        if msc.connect_to_device(dpg.get_value("device_list")):
            dpg.set_value("connection_state_text_field",
                          "Connection state: CONNECTED")
    except:
        dpg.show_item("connection_error_dialog")


def send_hello(msc):
    try:
        msc.send_hello(dpg.get_value("device_list"))
    except Exception as e:
        dpg.show_item("connection_error_dialog")
        print(e)


def send_command(msc):
    try:
        if dpg.get_value("command_input") != None or dpg.get_value("command_input") != "":
            msc.send_cmd(dpg.get_value("device_list"),
                         dpg.get_value("command_input"))
    except Exception as e:
        dpg.show_item("connection_error_dialog")
        print(e)

# Callback wich updates and stores the config file


def save_callback(sender, app_data, user_data):
    """
    Callback executed when a configuration is saved
    """
    print("save")
    print(user_data.get_last_filename())
    user_data.set_value("config_name", user_data.get_last_filename())
    for i in user_data.get_config():
        # i: key, value = user_data.get_value(i)
        if i != "config_name":
            user_data.set_value(i, dpg.get_value(i))
    user_data.write_config_file(
        f"configs/{user_data.get_value('config_name')}.json")


def exit_callback():
    exit(0)


def manage_file(sender, app_data, user_data):
    """
    Callback executed when a configuration is read
    """

    user_data.read_config_file(app_data['file_path_name'])
    user_data.set_last_filename(user_data.get_value("config_name"))
    dpg.set_value("current_config_open",
                  f"Motor configuration file used: {user_data.get_value('config_name')}")

    for i in user_data.get_config():
        # i: key, value = user_data.get_value(i)
        if i != "config_name":
            dpg.set_value(i, user_data.get_value(i))


def create_file(sender, app_data, user_data):
    """
    Callback executed when a configuration is created
    """
    user_data.init_config(app_data['file_name'].replace('.json', ''))
    user_data.set_last_filename(app_data['file_name'].replace('.json', ''))
    dpg.set_value("current_config_open",
                  f"Motor configuration file used: {user_data.get_value('config_name')}")
    # TODO: set to standard values?


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)
        print(f"\nSome errors occured: {e}\r\n")
