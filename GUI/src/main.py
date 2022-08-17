import gui
import micro_serial_handler
import plot
import queue
import sys
import os
if __name__ == "__main__":
    try:
        serial_slave_echo = queue.Queue()
        serial_master_echo = queue.Queue()
        micro_serial_handler_instance = micro_serial_handler.Micro_serial_handler()
        plot_instance = plot.Plot_Visualizer()
        micro_serial_handler_instance.config(
            serial_slave_echo, serial_master_echo)
        gui_instance = gui.GUI(micro_serial_handler_instance, plot_instance)
        gui_instance.config(serial_slave_echo, serial_master_echo)
        micro_serial_handler_instance.run()
        gui_instance.run()
    except Exception as e:
        micro_serial_handler_instance.close_connection("master")
        micro_serial_handler_instance.close_connection("slave")
        micro_serial_handler_instance.stop()
        micro_serial_handler_instance.join()
        gui_instance.join()
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)
        print(f"\nSome errors occured: {e}\r\n")
