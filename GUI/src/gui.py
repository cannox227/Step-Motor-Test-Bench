from io import StringIO
from datetime import datetime
import csv
import dearpygui.dearpygui as dpg
import plot as plot
import motor_config
import micro_serial_handler
import queue
import threading
import time
from random import random

# Global var
light_blue = (0, 153, 255)
motor_settings_win_pos = (400, 0)
serial_win_pos = (0, 440)
MAX_MOTOR_STEP_DIGITS = 6
MAX_ACQUISITION_TIME_DIGITS = 2


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


def load_parameters(motor_conf, msc):
    items_to_be_sent = []
    text_to_be_shown = ""
    counter = 0
    serialization_configuration = ""
    try:
        for i in motor_conf.get_config():
            # i: key, value = user_data.get_value(i)
            if i != "config_name":
                counter += 1
                transformed_element = motor_conf.get_value_by_key(
                    i, dpg.get_value(i))
                if transformed_element != -1:
                    items_to_be_sent.append(transformed_element)
                else:
                    items_to_be_sent.append(dpg.get_value(i))
                current_type = motor_conf.get_motor_param_type(
                    transformed_element, i)
                serialization_configuration += current_type
                print(f"type of ({i}) = {current_type}")
                support = (
                    f"{i}: {dpg.get_value(i)} -> {transformed_element}\r\n")
                # print(support)
                text_to_be_shown += support
                # print(i, dpg.get_value(i))
        print(items_to_be_sent)
        print(serialization_configuration)
        # Checksum
        print(f"List len: {len(items_to_be_sent)}, len config: {counter}")
        msc.custom_serialize_payload(
            items_to_be_sent, "slave", serialization_configuration)
        dpg.set_value("current_config_text", text_to_be_shown)
        dpg.show_item("current_config_dialog")
    except Exception as e:
        dpg.set_value("Connection error message", f"Error: {e}")
        dpg.show_item("connection_error_dialog")


def build_movement_cmd(val):
    str_val = str(val)
    zeros = MAX_MOTOR_STEP_DIGITS - len(str_val)
    return str(("0"*zeros)+str(val))


def build_acquisition_cmd(val):
    str_val = str(val)
    zeros = MAX_ACQUISITION_TIME_DIGITS - len(str_val)
    return str(("0"*zeros)+str(val))


def update_and_show_error_popup(e):
    dpg.set_value("Connection error message", f"Error: {e}")
    dpg.show_item("connection_error_dialog")
    # print(e)


def update_and_show_generic_error_popup(e):
    dpg.set_value("error_dialog_message", f"Error: {e}")
    dpg.show_item("error_dialog")


def update_and_show_generic_success_popup(e):
    dpg.set_value("success_dialog_message", f"Success: {e}")
    dpg.show_item("success_dialog")


def update_device_list(msc):
    msc.update_available_devices()
    dpg.configure_item(
        "master_device_list", items=msc.get_available_devices_by_name())
    dpg.configure_item(
        "slave_device_list", items=msc.get_available_devices_by_name())


def connect_to_device(msc, device_type):
    try:
        if device_type == "master":
            if msc.connect_to_device(dpg.get_value("master_device_list"), "master"):
                dpg.set_value("master_connection_state_text_field",
                              f'Connection state:\r\nCONNECTED TO {msc.get_device_name(dpg.get_value("master_device_list"))}')
        elif device_type == "slave":
            if msc.connect_to_device(dpg.get_value("slave_device_list"), "slave"):
                dpg.set_value("slave_connection_state_text_field",
                              f'Connection state:\r\nCONNECTED TO {msc.get_device_name(dpg.get_value("slave_device_list"))}')
    except Exception as e:
        update_and_show_error_popup(e)


def send_hello(msc, device_type):
    try:
        if device_type == "master":
            msc.send_hello(dpg.get_value("master_device_list"), "master")
        elif device_type == "slave":
            msc.send_hello(dpg.get_value("slave_device_list"), "slave")
    except Exception as e:
        update_and_show_error_popup(e)


def send_struct(msc, device_type):
    try:
        payload = [1.1, 2.2, 3.3, 4.4]
        msc.serialize_payload(payload, device_type)
    except Exception as e:
        update_and_show_error_popup(e)


def send_generic_command(msc, device_type, command):
    try:
        print(command)
        msc.send_cmd(command, device_type)
    except Exception as e:
        update_and_show_error_popup(e)


def send_command(msc, device_type):
    try:
        if device_type == "master":
            if dpg.get_value("master_command_input") != None or dpg.get_value("master_command_input") != "":
                msc.send_cmd(dpg.get_value("master_command_input"), "master")
                print(f'Sent: {dpg.get_value("master_command_input")}')
        elif device_type == "slave":
            if dpg.get_value("slave_command_input") != None or dpg.get_value("slave_command_input") != "":
                msc.send_cmd(dpg.get_value("slave_command_input"), "slave")
                print(f'Sent: {dpg.get_value("slave_command_input")}')
    except Exception as e:
        update_and_show_error_popup(e)


def send_brake_command(msc, cmd):
    try:
        msc.send_cmd(cmd, "master")
        # pwm_val = msc.read_line("master").decode()
        # print(pwm_val)
        # dpg.set_value(item="pwm_text_field",value=f"PWM: {float(pwm_val[4:8])*100}%")
    except Exception as e:
        update_and_show_error_popup(e)


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


def update_gui(msc):
    try:
        for i in msc.get_config():
            # print(i)
            # i: key, value = user_data.get_value(i)
            if i != "config_name":
                if i == "working_mode":
                    if msc.get_value(i) == "POWERSTEP01_CM_VM_VOLTAGE":
                        dpg.show_item("motor_settings_voltage_mode")
                        dpg.hide_item("motor_settings_current_mode")
                    elif msc.get_value(i) == "POWERSTEP01_CM_VM_CURRENT":
                        dpg.show_item("motor_settings_current_mode")
                        dpg.hide_item("motor_settings_voltage_mode")
                dpg.set_value(i, msc.get_value(i))
    except Exception as e:
        print(e)


def print_current_config(sender, app_data, user_data):
    for i in user_data.get_config():
        # i: key, value = user_data.get_value(i)
        if i != "config_name":
            print(f"{i}: {dpg.get_value(i)}")
            user_data.set_value(i, dpg.get_value(i))
    dpg.set_value("current_config_text",
                  str(user_data.get_config()).replace(',', '\r\n').replace('{', '').replace('}', ''))
    dpg.show_item("current_config_dialog")


def exit_callback():
    dpg.destroy_context()
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
    show_settings(sender, user_data.get_value("working_mode"))


def create_file(sender, app_data, user_data):
    """
    Callback executed when a configuration is created
    """
    user_data.init_config(app_data['file_name'].replace('.json', ''))
    user_data.set_last_filename(app_data['file_name'].replace('.json', ''))
    dpg.set_value("current_config_open",
                  f"Motor configuration file used: {user_data.get_value('config_name')}")
    # TODO: set to standard values?


def devices_selector_callback(sender, app_data, user_data):
    if sender == "master_device_list":
        user_data.set_selected_device(app_data, "master")
        # print(f'Selected {user_data.get_selected_device("master")}')
    elif sender == "slave_device_list":
        user_data.set_selected_device(app_data, "slave")
        # print(f'Selected {user_data.get_selected_device("slave")}')


def show_settings(sender, app_data):
    if app_data == "POWERSTEP01_CM_VM_VOLTAGE":
        dpg.show_item("motor_settings_voltage_mode")
        dpg.hide_item("motor_settings_current_mode")
    elif app_data == "POWERSTEP01_CM_VM_CURRENT":
        dpg.show_item("motor_settings_current_mode")
        dpg.hide_item("motor_settings_voltage_mode")


class GUI(threading.Thread):

    def __init__(self, msc_handler: micro_serial_handler.Micro_serial_handler, plot_handler: plot.Plot_Visualizer):
        self.msc_handler = msc_handler
        self.plot_handler = plot_handler
        self.file_buff = ""
        self.running = False
        # Graphs and GUI obj creation
        dpg.create_context()
        # with dpg.window(label="Motor Voltage plot", tag="Motor Voltage plot", pos=(300, 0), show=False):
        #     with dpg.plot(label="Voltage - w", height=300, width=300):
        #         dpg.add_plot_legend()
        #         dpg.add_plot_axis(dpg.mvXAxis, label="w")
        #         dpg.add_plot_axis(dpg.mvXAxis, label="V", tag="y-axis-V")
        #         dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        #         ), tag="V vals", label="V-w", parent="y-axis-V")

        # with dpg.window(label="Motor Torque plot", tag="Motor Torque plot", pos=(300, 150), show=False):
        #     with dpg.plot(label="Torque - w", height=300, width=300):
        #         dpg.add_plot_legend()
        #         dpg.add_plot_axis(dpg.mvXAxis, label="w")
        #         dpg.add_plot_axis(dpg.mvXAxis, label="T", tag="y-axis-T")
        #         dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        #         ), tag="T vals", label="T-w", parent="y-axis-T")

        # with dpg.window(label="Motor Power plot", tag="Motor Power plot", pos=(300, 50), show=False):
        #     with dpg.plot(label="Power - w", height=300, width=300):
        #         dpg.add_plot_legend()
        #         dpg.add_plot_axis(dpg.mvXAxis, label="w")
        #         dpg.add_plot_axis(dpg.mvXAxis, label="P", tag="y-axis-P")
        #         dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        #         ), tag="P vals", label="P-w", parent="y-axis-P")

        # with dpg.window(label="Motor Current plot", tag="Motor Current plot", pos=(300, 100), show=False):
        #     with dpg.plot(label="Current - w", height=300, width=300):
        #         dpg.add_plot_legend()
        #         dpg.add_plot_axis(dpg.mvXAxis, label="w")
        #         dpg.add_plot_axis(dpg.mvXAxis, label="I", tag="y-axis-I")
        #         dpg.add_line_series(plot.get_xvals(), plot.get_yvals(
        #         ), tag="I vals", label="I-w", parent="y-axis-I")

        # Connection error dialog
        with dpg.window(label="Connection error", tag="connection_error_dialog", pos=(300, 300), show=False):
            dpg.add_text("Serial connection error", tag="Connection error")
            dpg.add_text("Error message", tag="Connection error message")
            dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
                item="connection_error_dialog"))

        # Generic error dialog
        with dpg.window(label="Error", tag="error_dialog", pos=(300, 300), show=False):
            dpg.add_text("Error message", tag="error_dialog_message")
            dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
                item="error_dialog"))

        # Generic success dialog
        with dpg.window(label="Succes", tag="success_dialog", pos=(300, 300), show=False):
            dpg.add_text("Success message", tag="success_dialog_message")
            dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
                item="success_dialog"))

        # Config popup
            with dpg.window(label="Current config", tag="current_config_dialog", pos=(300, 300), show=False):
                dpg.add_text("", tag="current_config_text")
                dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
                    item="current_config_dialog"))

        # CSV plot popup
        with dpg.window(label="CSV PLOT", tag="csv_plot_window", pos=(300, 300), show=False):
            dpg.add_text("", tag="csv_plot_text")
            dpg.add_text("Plot preview")
            with dpg.plot(label="Torque - time", height=400, width=400):
                dpg.add_plot_legend()
                dpg.add_plot_axis(dpg.mvXAxis, label="time")
                dpg.add_plot_axis(dpg.mvXAxis, label="T", tag="y-axis-T")
                dpg.add_line_series([], [], tag="T vals",
                                    label="T-time", parent="y-axis-T")
            dpg.add_button(label="Save plot as image",
                           callback=lambda: self.save_plot())
            dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
                item="csv_plot_window"))

        # Create viewport where windows will be contained
        dpg.create_viewport(
            title='ProM Motor driver configurator', x_pos=0, y_pos=0)

        # import classes
        # msc_handler = micro_serial_handler.Micro_serial_handler()
        motor_configuration = motor_config.Motor_Config()

        # import JSON config file
        motor_configuration.read_config_file("configs/conf01.json")

        # File manager read
        with dpg.file_dialog(directory_selector=False, show=False, user_data=motor_configuration, callback=manage_file, tag="file_dialog_r", height=50):
            dpg.add_file_extension(".json")

        # File manager write
        with dpg.file_dialog(directory_selector=False, show=False, user_data=motor_configuration, callback=create_file, tag="file_dialog_c", height=50):
            dpg.add_file_extension(".json")

        # Motor settings window
        with dpg.window(label="Motor settings", tag="Motor settings"):

            with dpg.menu_bar():
                with dpg.menu(label="File"):
                    dpg.add_menu_item(label="New config file",
                                      callback=lambda: dpg.show_item("file_dialog_c"))
                    dpg.add_menu_item(label="Open config file",
                                      callback=lambda: dpg.show_item("file_dialog_r"))

            dpg.add_text(
                f"Motor configuration file used: {motor_configuration.get_value('config_name')}", tag="current_config_open")

            with dpg.group(horizontal=True) as main_panel:

                with dpg.group(horizontal=False) as motor_settings:

                    dpg.add_text("Common settings", color=light_blue)
                    # Acceleration
                    dpg.add_input_float(tag="acceleration_rate", label="Acceleration_rate [step/s2] (min: 14.55, max: 59590)",
                                        default_value=14.55, max_value=59590, min_value=14.55, min_clamped=True, max_clamped=True, width=100)
                    # Deceleration
                    dpg.add_input_float(tag="deceleration_rate", label="Deceleration_rate [step/s2] (min: 14.55, max: 59590)",
                                        default_value=14.55, max_value=59590, min_value=14.55, min_clamped=True, max_clamped=True, width=100)
                    # Max speed
                    dpg.add_input_float(
                        tag="max_speed", label="Max speed [step/s] (min: 15.25, max: 15610)", default_value=15.25, max_value=15610, min_value=15.25, min_clamped=True, max_clamped=True, width=100)
                    # Min speed
                    dpg.add_input_float(
                        tag="min_speed", label="Min speed [step/s] (min: 0, max: 976.3)", default_value=0, max_value=976.3, min_value=0, min_clamped=True, max_clamped=True, width=100)
                    # Low speed optimization bit
                    dpg.add_combo(tag="low_speed_optimization_bit", label="Low speed optimization bit",
                                  items=motor_configuration.get_LSPD_bit_keys(), width=200)
                    # Fullstep speed
                    dpg.add_input_float(
                        tag="full_step_speed", label="Full step speed [step/s] (min: 7.63, max: 15625)", default_value=7.63, max_value=15625, min_value=7.63, min_clamped=True, max_clamped=True, width=100)
                    # Boost mode bit
                    dpg.add_combo(tag="boost_mode_bit", label="Boost mode bit",
                                  items=motor_configuration.get_BOOST_mode_keys(), width=200)
                    # Over current detection threshold
                    dpg.add_input_float(tag="overcurrent_threshold",
                                        label="Overcurrent threshold", default_value=281.25,  min_clamped=True, max_clamped=True, width=100)
                    # Step mode
                    dpg.add_combo(tag="step_mode", label="Step mode",
                                  items=motor_configuration.get_STEP_MODE_keys(), width=200)
                    # Gate sink/source current used by gate driving circuitry
                    dpg.add_combo(tag="gate_sink_source_current", label="Gate sink/source current",
                                  width=200, items=motor_configuration.get_I_GATE_keys())
                    # Duration of the overboost phase during gate turn-off
                    dpg.add_combo(tag="turn_off_time_boost", label="Turn off time boost",
                                  width=200, items=motor_configuration.get_TBOOST_keys())
                    # Duration of constant current phase during gate turn-on and turn-off
                    dpg.add_combo(tag="tcc", label="TCC", width=200,
                                  items=motor_configuration.get_TCC_keys())
                    # External clock watchdog
                    dpg.add_combo(tag="clock_wd_enable", label="Ext CLK watchdog",
                                  width=200, items=motor_configuration.get_WD_EN_keys())
                    # Blanking time
                    dpg.add_combo(tag="blanking_time", label="Blanking time",
                                  width=200, items=motor_configuration.get_TBLANK_keys())
                    # Dead time
                    dpg.add_combo(tag="dead_time", label="Dead time",
                                  width=200, items=motor_configuration.get_TDT_keys())

                with dpg.group(horizontal=False) as specific_operating_mode_panel:
                    dpg.add_text("Operating mode", color=light_blue)
                    dpg.add_radio_button(tag="working_mode", horizontal=True, items=motor_configuration.get_CM_VM_keys(
                    ), default_value="Voltage mode", callback=show_settings)
                    # Motor settings in current mode
                    with dpg.group(xoffset=10, tag="motor_settings_current_mode", show=False) as motor_settings_current_mode:
                        # Current hold torque
                        dpg.add_input_float(
                            tag="current_hold_torque_mv", label="Hold torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                        # Current running torque
                        dpg.add_input_float(
                            tag="current_running_torque_mv", label="Running torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                        # Current acceleration torque
                        dpg.add_input_float(
                            tag="current_acceleration_torque_mv", label="Acceleration torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                        # Current deceleration torque
                        dpg.add_input_float(
                            tag="current_deceleration_torque_mv", label="Deceleration torque [mV] (min: 7.8, max: 1000)", default_value=7.8, max_value=1000, min_value=7.8, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                        # Maximum fast decay time
                        dpg.add_combo(tag="current_max_decay_time_us", label="Maximum fast decay time [us]",
                                      items=motor_configuration.get_TOFF_FAST_keys(), width=200)
                        # Maximum fall step time
                        dpg.add_combo(tag="current_max_fall_time_us", label="Maximum fall step time [us]",
                                      items=motor_configuration.get_FAST_STEP_keys(), width=200)
                        # Minimum on-time
                        dpg.add_input_float(
                            tag="current_min_on_time_us", label="Minmum on-time [us] (min: 0.5, max: 64)", default_value=0.5, max_value=64, min_value=0.5, format="%.2f", min_clamped=True, max_clamped=True, width=100)
                        # Maximum off-time
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
                    dpg.add_separator()

                    with dpg.group(horizontal=False) as main_panel_actions:
                        dpg.add_text("Actions", color=light_blue)
                        dpg.add_button(label="Load parameters into Powerstep",
                                       callback=lambda: load_parameters(motor_configuration, self.msc_handler))

                        with dpg.group(horizontal=True) as plot_params_VT:
                            dpg.add_button(label="End Acquisition and store to file",
                                           callback=lambda: self.update_csv_buffer())
                        dpg.add_text("Last acquisition file created: ",
                                     tag="last_acquisition_field")
                        # dpg.add_button(label="Plot CSV",
                        #                callback=lambda: show_csv_window())
                        # dpg.add_button(label="Plot V-t",
                        #                callback=lambda: plot_V())
                        #     dpg.add_button(label="Plot T-t",
                        #                    callback=lambda: self.save_plot())
                        # with dpg.group(horizontal=True) as plot_params_PI:
                        #     dpg.add_button(label="Plot P-t",
                        #                    callback=lambda: plot_P())
                        #     dpg.add_button(label="Plot I-t",
                        #                    callback=lambda: plot_I())
                        dpg.add_separator()

                        with dpg.group(horizontal=True) as bottom_buttons:
                            dpg.add_button(label="Save with override",
                                           callback=save_callback, user_data=motor_configuration)
                            dpg.add_button(label="Print current config",
                                           callback=print_current_config, user_data=motor_configuration)
                            dpg.add_button(
                                label="Exit", callback=exit_callback)

                # Serial window
                with dpg.window(label="Serial interface", tag="Serial interface", pos=serial_win_pos, width=dpg.get_viewport_max_width()):
                    dpg.add_text("Serial interface", color=light_blue)
                    dpg.add_button(label="Refresh device list",
                                   callback=lambda: update_device_list(self.msc_handler))

                    with dpg.group(horizontal=True) as serial_panel:
                        with dpg.group(horizontal=False) as master_panel:
                            # Master selector
                            dpg.add_text("Available devices to be Master")
                            dpg.add_combo(items=self.msc_handler.get_available_devices(),
                                          tag="master_device_list", callback=devices_selector_callback, user_data=self.msc_handler, width=300)
                            dpg.add_button(label="CONNECT TO MASTER",
                                           callback=lambda: connect_to_device(self.msc_handler, "master"))
                            dpg.add_text("Connection state: NOT CONNECTED",
                                         tag="master_connection_state_text_field")
                            dpg.add_input_int(label="Automatic test time duration (s)", tag="automatic-test-duration",
                                              min_clamped=True, max_clamped=True, min_value=5, max_value=60, width=100)
                            with dpg.group() as automatic_test_section:
                                dpg.add_button(label="START AUTOMATIC MOTOR TEST",
                                               callback=lambda: send_generic_command(self.msc_handler, "master", f"start_motor+{build_acquisition_cmd(dpg.get_value('automatic-test-duration'))}"))
                            # dpg.add_button(label="Send hello to master",
                            #                callback=lambda: send_hello(self.msc_handler, "master"))
                            with dpg.group() as master_cmd_input_section:
                                dpg.add_text("Master custom command input")
                                dpg.add_input_text(
                                    tag="master_command_input", width=100)
                                dpg.add_button(label="SEND COMMAND TO MASTER",
                                               callback=lambda: send_command(self.msc_handler, "master"))
                                # dpg.add_button(label="SEND STRUCT TO MASTER",
                                #                callback=lambda: send_struct(self.msc_handler, "master"))
                        with dpg.group(horizontal=False) as master_log:
                            dpg.add_text("Master serial display",
                                         color=light_blue)
                            dpg.add_input_text(tag="master_serial_display", readonly=True,
                                               multiline=True, tracked=True, width=300, height=200)
                        with dpg.group(horizontal=False) as slave_panel:
                            # Slave selector
                            dpg.add_text("Available devices to be Slave")
                            dpg.add_combo(items=self.msc_handler.get_available_devices(),
                                          tag="slave_device_list", callback=devices_selector_callback, user_data=self.msc_handler, width=300)
                            dpg.add_button(label="CONNECT TO SLAVE",
                                           callback=lambda: connect_to_device(self.msc_handler, "slave"))
                            dpg.add_text("Connection state: NOT CONNECTED",
                                         tag="slave_connection_state_text_field")
                            # dpg.add_button(label="Send hello to slave",
                            #                callback=lambda: send_hello(self.msc_handler, "slave"))
                            with dpg.group() as slave_cmd_input_section:
                                dpg.add_text("Slave custom command input")
                                dpg.add_input_text(
                                    tag="slave_command_input", width=100)
                                dpg.add_button(label="SEND COMMAND TO SLAVE",
                                               callback=lambda: send_command(self.msc_handler, "slave"))
                                # dpg.add_button(label="SEND STRUCT TO SLAVE",
                                #                callback=lambda: send_struct(self.msc_handler, "slave"))
                        with dpg.group(horizontal=False) as slave_log:
                            dpg.add_text("Slave serial display",
                                         color=light_blue)
                            dpg.add_input_text(tag="slave_serial_display", readonly=True,
                                               multiline=True, tracked=True, width=300, height=200)

        # Manual interface
        with dpg.window(label="Manual interface", tag="Manual interface", pos=(dpg.get_viewport_width()-350, 0)):
            # Motor control
            with dpg.group(horizontal=False) as motor_control_params:
                dpg.add_text("Motor control", color=light_blue)
                dpg.add_text("Input steps", tag="input_steps")
                dpg.add_input_int(tag="motor_steps_to_do", default_value=0, min_value=0,
                                  max_value=999999, width=100, max_clamped=True, min_clamped=True)
                with dpg.group(horizontal=True) as motor_control_buttons_section_A:
                    dpg.add_button(label="Move forward", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "fw+"+build_movement_cmd(dpg.get_value("motor_steps_to_do"))))
                    dpg.add_button(label="Move backward", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "bw+"+build_movement_cmd(dpg.get_value("motor_steps_to_do"))))
                    dpg.add_button(label="Go to position", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "gt+"+build_movement_cmd(dpg.get_value("motor_steps_to_do"))))
                with dpg.group(horizontal=True) as motor_control_buttons_section_B:
                    dpg.add_button(label="Go home", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "gh+000000"))
                    dpg.add_button(label="Motor stop", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "st+000000"))
                    dpg.add_button(label="Motor hold", callback=lambda: send_generic_command(
                        self.msc_handler, "slave", "hd+000000"))
            # Brake window
            with dpg.group() as brake_window:
                dpg.add_text("Brake control", color=light_blue)
                with dpg.group(horizontal=False) as brake_panel:
                    dpg.add_text("PWM: 0%", tag="pwm_text_field")
                    with dpg.group(horizontal=True) as brake_subpanel_A:
                        dpg.add_button(
                            label="Brake +", callback=lambda: send_brake_command(self.msc_handler, "brake+"))
                        dpg.add_button(
                            label="Brake -", callback=lambda: send_brake_command(self.msc_handler, "brake-"))
                    with dpg.group(horizontal=True) as brake_subpanel_B:
                        dpg.add_button(
                            label="Brake MAX", callback=lambda: send_brake_command(self.msc_handler, "brake_on_max"))
                        dpg.add_button(
                            label="Brake OFF", callback=lambda: send_brake_command(self.msc_handler, "brake_off"))
            with dpg.group() as async_reading_panel:
                dpg.add_text("Sensor acquisition control", color=light_blue)
                with dpg.group(horizontal=True) as acquisition_subpanel_A:
                    dpg.add_button(
                        label="Print 3 acquired samples", callback=lambda: send_generic_command(self.msc_handler, "master", "get_acquired_data_chunks"))

        # Update gui with parameters
        update_gui(motor_configuration)

    def config(self, slave_echo: queue.Queue, master_echo: queue.Queue):
        self.slave_echo = slave_echo
        self.master_echo = master_echo

    def update_csv_buffer(self):
        if self.file_buff != "":
            try:
                header = ['Sender', 'Time [ms]', 'Torque [Nm]',
                          'Voltage [V]', 'Current [A]', 'Brake [%]']
                buffer = StringIO(self.file_buff)
                reader = csv.reader(buffer, skipinitialspace=True)
                file_name = f'{datetime.now().strftime("%d-%m-%Y_%H-%M-%S")}.csv'
                self.plot_handler.last_plot_name = file_name
                file_path = f'csv/{file_name}'
                dpg.set_value("csv_plot_text", "CSV UPDATED!\n"+file_name)
                with open(file_path, 'w', newline='') as f:
                    writer = csv.writer(f)
                    writer.writerow(header)
                    writer.writerows(reader)
                self.plot_handler.read_csv_vals(file_path)
                print(f"x vals {self.plot_handler.get_xvals()}")
                print(f"y vals {self.plot_handler.get_torque_vals()}")
                update_and_show_generic_success_popup(
                    f"Data stored in: csv/{file_name}")
                dpg.set_value("last_acquisition_field",
                              f"Last acquisition file created: {file_name}")
            except Exception as e:
                update_and_show_generic_error_popup(e)
        else:
            update_and_show_generic_error_popup(
                "Can't store in a file! No data available")

    def add_to_file_buff(self, payload):
        self.file_buff += payload+"\n"

    def serial_slave_listener_callback(self):
        while self.running:
            try:
                msg = self.slave_echo.get(block=False)
                if not msg:
                    pass
                else:
                    msg = msg.replace("\n", "").replace(
                        "\r", "").replace("", "")
                    msg = f'{msg}\n{dpg.get_value("slave_serial_display")}'
                    try:
                        dpg.set_value("slave_serial_display", f"{msg}")
                    except Exception as e:
                        print(e)
                    time.sleep(0.05)
            except queue.Empty:
                # dpg.set_value("slave_serial_display", "DEVICE NOT CONNECTED")
                # print("Slave: Queue empty")
                time.sleep(0.05)
            except Exception as e:
                # print(f"Slave: Error in periodic serial listener {e}")
                time.sleep(0.05)

    def serial_master_listener_callback(self):
        while self.running:
            try:
                msg = self.master_echo.get(block=False)
                if not msg:
                    pass
                else:
                    msg = msg.replace("\n", "").replace(
                        "\r", "").replace("", "")
                    if "PWM+" in msg:
                        # update_and_show_generic_success_popup(msg)
                        print(f"PWM from master => {msg}")
                        value = (f"PWM: {float(msg[4:8])*100}%")
                        dpg.set_value("pwm_text_field", value)
                    elif "[MASTER][RECEIVED]" not in msg and "[MASTER][READING ENDED]" not in msg and "[MASTER][READING STARTED]" not in msg:
                        self.file_buff += msg+"\n"

                    msg = f'{msg}\n{dpg.get_value("master_serial_display")}'
                    try:
                        dpg.set_value("master_serial_display", f"{msg}")
                    except Exception as e:
                        print(e)
                    time.sleep(0.05)
            except queue.Empty:
                # dpg.set_value("master_serial_display", "DEVICE NOT CONNECTED")
                # print("Master: Queue empty")
                time.sleep(0.05)
            except Exception as e:
                # print(f"Master: Error in periodic serial listener {e}")
                time.sleep(0.05)

    def run(self):

        self.running = True
        # Fill the viewport with a specifig window using its tag
        # dpg.set_primary_window("Motor settings", True)
        # Necessary setup phase
        dpg.setup_dearpygui()
        # Create and start the thread over serial_listener_callback
        self.serial_slave_listener = threading.Thread(
            target=self.serial_slave_listener_callback)
        self.serial_slave_listener.start()
        self.serial_master_listener = threading.Thread(
            target=self.serial_master_listener_callback)
        self.serial_master_listener.start()
        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()
        self.msc_handler.is_running = False
        self.msc_handler.serial_slave_listener.join()
        self.msc_handler.serial_master_listener.join()
        self.running = False
        self.serial_slave_listener.join()
        self.serial_master_listener.join()
        # join thread
