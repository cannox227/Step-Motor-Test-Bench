import serial.tools.list_ports
import io


class Micro_serial_handler():

    def __init__(self):
        self.available_devices = []
        self.serial_socket = None
        self.selected_device = None
        self.last_cmd_sent = "none"

    def update_available_devices(self):
        self.available_devices = list(serial.tools.list_ports.comports())
        return self.available_devices

    def get_available_devices(self):
        return self.available_devices

    def get_available_devices_by_name(self):
        return list([i.device for i in self.available_devices])

    def connect_to_device(self, device_name):
        self.selected_device = device_name
        self.serial_socket = serial.Serial(
            device_name, baudrate=115200, timeout=1)
        return self.serial_socket.is_open

    def send_hello(self, device_name):
        if self.serial_socket is not None or self.serial_socket.is_open:
            self.serial_socket.write(
                ("HELLO FROM DEAR PY GUI \r\n").encode('utf-8'))
        else:
            print("ERROR: Device not connected!")
