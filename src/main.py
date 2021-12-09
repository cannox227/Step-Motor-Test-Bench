import dearpygui.dearpygui as dpg
import dearpygui.demo as demo
import plot as plot
import config

# Get value of a numeric slider
def get_slider_value(sender):
    return str(round(dpg.get_value(sender),2))

# Callback wich updates and stores the config file
def save_callback(sender, app_data, user_data):

    hold = float(get_slider_value('hold'))
    run = float(get_slider_value('run'))
    acc = float(get_slider_value('acc'))
    dec = float(get_slider_value('dec'))

    config.set_value('hold_dt', hold)
    config.set_value('run_dt', run)
    config.set_value('acc', acc)
    config.set_value('dec', dec)
        
    print(f"Hold DT is: {hold}")
    print(f"Run DT is: {run}")
    print(f"Acceleration is: {acc}")
    print(f"Deceleration is: {dec}")

def main():

    #import JSON config file
    config.read_config()
    
    dpg.create_context()

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
        dpg.add_text(f"Motor configuration file: {config.get_value('config_name')}")
        with dpg.group() as settings:
            dpg.add_slider_float(tag="hold", label="Hold DT [%]", default_value=config.get_value('hold_dt'), max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="run", label="Run DT [%]", default_value=config.get_value('run_dt'),max_value=99.6, width=100, format="%.2f")
            dpg.add_slider_float(tag="acc", label="Acceleration", default_value=config.get_value('acc'), width=100, max_value=99.6, format="%.2f")
            dpg.add_slider_float(tag="dec", label="Deceleration", default_value=config.get_value('dec'), max_value=99.6, width=100, format="%.2f")
            with dpg.group() as group1:
                pass

        dpg.add_button(label="Save", callback=save_callback)

    with dpg.window(label="Motor Torque plot",tag="Motor Torque plot", pos=(300, 0)):
        with dpg.plot(label="Torque - w", height=300, width=300):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="w")
            dpg.add_plot_axis(dpg.mvXAxis, label="T", tag="y-axis")
            dpg.add_line_series(plot.get_xvals(), plot.get_yvals(), label="T-w", parent="y-axis")

    # Create viewport where windows will be contained
    dpg.create_viewport(title='ProM Motor driver configurator', width=800, height=800)
    # Fill the viewport with a specifig window using its tag
    #dpg.set_primary_window("Motor settings", True)
    # Necessary setup phase
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

if __name__ == "__main__":
    main()