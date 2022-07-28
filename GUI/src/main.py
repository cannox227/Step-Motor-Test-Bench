from readline import append_history_file
from traceback import print_tb
import dearpygui.dearpygui as dpg
from matplotlib.style import available
import plot as plot
import config
import micro_serial_handler

# Get value of a numeric slider


def get_slider_value(sender):
    return str(round(dpg.get_value(sender), 2))

# Get radio value


def get_radio_value(sender):
    return str(dpg.get_value(sender))

# Callback wich updates and stores the config file


def save_callback(sender, app_data, user_data):

    hold = float(get_slider_value('hold'))
    run = float(get_slider_value('run'))
    acc = float(get_slider_value('acc'))
    dec = float(get_slider_value('dec'))
    step = get_radio_value('step')
    rotation = get_radio_value('rotation')

    config.set_value('hold_dt', hold)
    config.set_value('run_dt', run)
    config.set_value('acc', acc)
    config.set_value('dec', dec)
    config.set_value('step', step)
    config.set_value('rotation', rotation)

    # print(f"Hold DT is: {hold}")
    # print(f"Run DT is: {run}")
    # print(f"Acceleration is: {acc}")
    # print(f"Deceleration is: {dec}")

    config.write_config(f"configs/{config.get_value('config_name')}.json")


def exit_callback():
    exit(0)


def manage_file(sender, app_data):
    config.read_config(app_data['file_path_name'])
    dpg.set_value("hold", config.get_value('hold_dt'))
    dpg.set_value("run", config.get_value('run_dt'))
    dpg.set_value("acc", config.get_value('acc'))
    dpg.set_value("dec", config.get_value('dec'))
    dpg.set_value("step", config.get_value('step'))
    dpg.set_value("rotation", config.get_value('rotation'))


def create_file(sender, app_data):
    config.init_config(app_data['file_name'].replace('.json', ''))
    # print(app_data['file_name'])
    config.write_config(app_data['file_path_name'])
    dpg.set_value("hold", config.get_value('hold_dt'))
    dpg.set_value("run", config.get_value('run_dt'))
    dpg.set_value("acc", config.get_value('acc'))
    dpg.set_value("dec", config.get_value('dec'))


def devices_selector_callback(sender, app_data):
    print(app_data)


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

# File manager read
with dpg.file_dialog(directory_selector=False, show=False, callback=manage_file, tag="file_dialog_r", height=50):
    dpg.add_file_extension(".json")

# File manager write
with dpg.file_dialog(directory_selector=False, show=False, callback=create_file, tag="file_dialog_c", height=50):
    dpg.add_file_extension(".json")

# Connection error dialog
with dpg.window(label="Connection error", tag="connection_error_dialog", pos=(300, 300), show=False):
    dpg.add_text("Serial connection error", tag="Connection error")
    dpg.add_button(label="OK", callback=lambda: dpg.hide_item(
        item="connection_error_dialog"))


def main():

    # import JSON config file
    config.read_config("configs/motor_params.json")

    # import classes
    msc_handler = micro_serial_handler.Micro_serial_handler()
    # msc_handler.update_available_devices()
    # print(f"!!{msc_handler.get_available_devices_by_name()}")

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

        dpg.add_text("Motor settings")
        dpg.add_text(label="")

        dpg.add_text(
            f"Motor configuration file: {config.get_value('config_name')}")
        with dpg.group() as motor_settings:
            dpg.add_slider_float(tag="hold", label="Hold DT [%]", default_value=config.get_value(
                'hold_dt'), max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="run", label="Run DT [%]", default_value=config.get_value(
                'run_dt'), max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="acc", label="Acceleration", default_value=config.get_value(
                'acc'), width=100, max_value=99.6, format="%.2f")
            dpg.add_slider_float(tag="dec", label="Deceleration", default_value=config.get_value(
                'dec'), max_value=99.6, width=100, format="%.2f")

            dpg.add_text(label="")
            dpg.add_radio_button(tag="step", horizontal=True, items=[
                                 "Half Step", "Full Step"], default_value=config.get_value('step'))
            dpg.add_radio_button(tag="rotation", horizontal=True, items=[
                                 "Clockwise", "Anti-Clockwise"], default_value=config.get_value('rotation'))

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
                               callback=save_callback)
                dpg.add_button(label="Exit", callback=exit_callback)

    # Create viewport where windows will be contained
    dpg.create_viewport(
        title='ProM Motor driver configurator', width=800, height=800)
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


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"\nSome errors occured: {e}\r\n")
