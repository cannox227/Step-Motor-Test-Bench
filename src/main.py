import dearpygui.dearpygui as dpg
import plot as plot
import config

dpg.create_context()
# Graphs
with dpg.window(label="Motor Voltage plot",tag="Motor Voltage plot", pos=(300, 0), show=False):
        with dpg.plot(label="Voltage - w", height=300, width=300):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="w")
            dpg.add_plot_axis(dpg.mvXAxis, label="V", tag="y-axis-V")
            dpg.add_line_series(plot.get_xvals(), plot.get_yvals(), tag="V vals", label="V-w", parent="y-axis-V")

with dpg.window(label="Motor Torque plot",tag="Motor Torque plot", pos=(300, 150), show=False):
        with dpg.plot(label="Torque - w", height=300, width=300):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="w")
            dpg.add_plot_axis(dpg.mvXAxis, label="T", tag="y-axis-T")
            dpg.add_line_series(plot.get_xvals(), plot.get_yvals(), tag="T vals",label="T-w", parent="y-axis-T")

with dpg.window(label="Motor Power plot",tag="Motor Power plot", pos=(300, 50), show=False):
        with dpg.plot(label="Power - w", height=300, width=300):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="w")
            dpg.add_plot_axis(dpg.mvXAxis, label="P", tag="y-axis-P")
            dpg.add_line_series(plot.get_xvals(), plot.get_yvals(), tag="P vals", label="P-w", parent="y-axis-P")

with dpg.window(label="Motor Current plot",tag="Motor Current plot", pos=(300, 100), show=False):
        with dpg.plot(label="Current - w", height=300, width=300):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="w")
            dpg.add_plot_axis(dpg.mvXAxis, label="I", tag="y-axis-I")
            dpg.add_line_series(plot.get_xvals(), plot.get_yvals(), tag="I vals", label="I-w", parent="y-axis-I")

# Get value of a numeric slider
def get_slider_value(sender):
    return str(round(dpg.get_value(sender),2))

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

    print(f"Hold DT is: {hold}")
    print(f"Run DT is: {run}")
    print(f"Acceleration is: {acc}")
    print(f"Deceleration is: {dec}")

    config.write_config()

def exit_callback():
    exit(0)

def main():

    #import JSON config file
    config.read_config()
    
    with dpg.window(label="Motor settings",tag="Motor settings"):

        with dpg.menu_bar():
            with dpg.menu(label="File"):
                dpg.add_menu_item(label="Open")
                dpg.add_menu_item(label="Import")
                dpg.add_menu_item(label="Export")

            with dpg.menu(label="Edit"):
                dpg.add_menu_item(label="Copy")
                dpg.add_menu_item(label="Paste")
                dpg.add_menu_item(label="Cut")
        

        dpg.add_text("Motor settings")
        dpg.add_text(label="")
        
        dpg.add_text(f"Motor configuration file: {config.get_value('config_name')}")
        with dpg.group() as motor_settings:
            dpg.add_slider_float(tag="hold", label="Hold DT [%]", default_value=config.get_value('hold_dt'), max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="run", label="Run DT [%]", default_value=config.get_value('run_dt'),max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="acc", label="Acceleration", default_value=config.get_value('acc'), width=100, max_value=99.6, format="%.2f")
            dpg.add_slider_float(tag="dec", label="Deceleration", default_value=config.get_value('dec'), max_value=99.6, width=100, format="%.2f")
            
            dpg.add_text(label="")
            dpg.add_radio_button(tag="step", horizontal=True, items=["Half Step", "Full Step"], default_value=config.get_value('step'))
            dpg.add_radio_button(tag="rotation", horizontal=True, items=["Clockwise", "Anti-Clockwise"], default_value=config.get_value('rotation'))
            
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
                dpg.add_button(label="Save", callback=save_callback)
                dpg.add_button(label="Exit", callback=exit_callback)

    

    # Create viewport where windows will be contained
    dpg.create_viewport(title='ProM Motor driver configurator', width=800, height=800)
    # Fill the viewport with a specifig window using its tag
    #dpg.set_primary_window("Motor settings", True)
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
    #dpg.get_item_configuration("power_plot")
    #dpg.configure_item("power_plot", show=True)
    ## graphs
    #TODO
    #modal=TRUE
    #implementare window come global
    


def plot_I():
    plot.plot_I()
    dpg.set_value("I vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Current plot")
    
def plot_T():
    plot.plot_T()
    dpg.set_value("T vals", [plot.get_xvals(), plot.get_yvals()])
    dpg.show_item("Motor Torque plot")
    


if __name__ == "__main__":
    main()