import dearpygui.dearpygui as dpg
import plot


class CSV_to_Plot_GUI():
    def __init__(self, plot_handler: plot.Plot_Visualizer) -> None:
        self.plot_handler = plot_handler
        self.last_plot_name = ""
        dpg.create_context()
        dpg.create_viewport(title='csv to plot converter')

        with dpg.window(label="CSV to image plot converter", height=400, width=400):
            with dpg.menu_bar():
                with dpg.menu(label="File"):
                    dpg.add_menu_item(label="Open csv file",
                                      callback=lambda: dpg.show_item("csv_file_dialog"))
        # File manager read
            with dpg.file_dialog(directory_selector=False, show=False, callback=self.manage_file, tag="csv_file_dialog", height=400):
                dpg.add_file_extension(".csv")

            dpg.add_text(
                "Select a csv file and then press the show button to display the plots")
            dpg.add_text("Selected file: ", tag="selected_file_text")
            dpg.add_text("File preview:", tag="selected_file_preview")
            dpg.add_button(label="Show",
                           callback=lambda: self.convert_to_plot())

            dpg.add_separator()
            dpg.add_text(
                "Max Torque value: 0 [Nm]", tag="torque_max_value_field")
            dpg.add_text(
                "Max Voltage value: 0 [V]", tag="voltage_max_value_field")
            dpg.add_text(
                "Max Current value: 0 [A]", tag="current_max_value_field")
            dpg.add_text("Max Power value: 0 [W]", tag="power_max_value_field")

        # Note for every plot use max 3 y axis (the fourth will be not assigned and with 5 a segmentation fault will occur)
        with dpg.window(label="CSV to image plot converter", tag="plot_window_0", pos=(400, 0), show=False,  width=400, height=400):

            with dpg.plot(label="Torque-Current-Voltage", height=400, width=-1):
                dpg.add_plot_legend()

                # create x axis
                dpg.add_plot_axis(dpg.mvXAxis, label="Time")

                # create y axis 1: Torque
                dpg.add_plot_axis(dpg.mvYAxis, label="Torque [Nm]")
                dpg.add_line_series((), (), label="Torque",
                                    parent=dpg.last_item(), tag="data_torque_0")

                # # create y axis 2: Current
                dpg.add_plot_axis(dpg.mvYAxis, label="Current [A]")
                dpg.add_line_series((), (), label="Current",
                                    parent=dpg.last_item(), tag="data_current")

                # # create y axis 3: Voltage
                dpg.add_plot_axis(dpg.mvYAxis, label="Voltage [V]")
                dpg.add_line_series((), (), label="Voltage",
                                    parent=dpg.last_item(), tag="data_voltage")

         # Note for every plot use max 3 y axis (the fourth will be not assigned and with 5 a segmentation fault will occur)
        with dpg.window(label="CSV to image plot converter", tag="plot_window_1", pos=(400, 0), show=False,  width=400, height=400):

            with dpg.plot(label="Torque-Power-Brake", height=400, width=-1):
                dpg.add_plot_legend()

                # create x axis
                dpg.add_plot_axis(dpg.mvXAxis, label="Time")

                # create y axis 1: Torque
                dpg.add_plot_axis(dpg.mvYAxis, label="Torque [Nm]")
                dpg.add_line_series((), (), label="Torque",
                                    parent=dpg.last_item(), tag="data_torque_1")

                # create y axis 4: Brake
                dpg.add_plot_axis(dpg.mvYAxis, label="Brake [%]")
                dpg.add_line_series((), (), label="Brake",
                                    parent=dpg.last_item(), tag="data_brake")

                # create y axis 5: Power
                dpg.add_plot_axis(dpg.mvYAxis, label="Power [W]")
                dpg.add_line_series((), (), label="Power",
                                    parent=dpg.last_item(), tag="data_power")

        dpg.setup_dearpygui()
        dpg.show_viewport()
        dpg.start_dearpygui()
        dpg.destroy_context()

    def manage_file(self, sender, app_data, user_data):
        """
        Callback executed when a csv file is read
        """
        self.last_plot_name = {app_data['file_name']}
        self.plot_handler.read_csv_vals(app_data['file_path_name'])
        dpg.set_value("selected_file_text",
                      f"Selected file: : {self.last_plot_name}")
        dpg.set_value("selected_file_preview",
                      f"File preview:\n{self.plot_handler.get_csv_head()}\n...\n{self.plot_handler.get_csv_tail()}")
        dpg.set_value("torque_max_value_field",
                      f"Max Torque value: {self.plot_handler.get_torque_max()} [Nm]")
        dpg.set_value("voltage_max_value_field",
                      f"Max Voltage value: {self.plot_handler.get_voltage_max()} [V]")
        dpg.set_value("current_max_value_field",
                      f"Max Current value: {self.plot_handler.get_current_max()} [A]")
        dpg.set_value("power_max_value_field",
                      f"Max Power value: {self.plot_handler.get_power_max()} [W]")

    def convert_to_plot(self):

        dpg.set_value("data_torque_0", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_torque_vals()])
        dpg.set_value("data_torque_1", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_torque_vals()])
        dpg.set_value("data_brake", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_brake_vals()])
        dpg.set_value("data_current", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_current_vals()])
        dpg.set_value("data_voltage", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_voltage_vals()])
        # print(f"PWR:{self.plot_handler.get_power_vals()}")
        dpg.set_value("data_voltage", [
            self.plot_handler.get_xvals(), self.plot_handler.get_voltage_vals()])
        dpg.set_value("data_power", [
                      self.plot_handler.get_xvals(), self.plot_handler.get_power_vals()])
        dpg.show_item("plot_window_0")
        dpg.show_item("plot_window_1")


if __name__ == "__main__":
    try:
        plot_visualizer_instance = plot.Plot_Visualizer()
        csv_to_plot_gui_instance = CSV_to_Plot_GUI(plot_visualizer_instance)
    except Exception as e:
        print(f"\nSome errors occured: {e}\r\n")
