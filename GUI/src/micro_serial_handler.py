from doctest import master
import serial.tools.list_ports
import time
import queue
from struct import *
import threading


class Micro_serial_handler(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)
        self.is_running = False
        self.available_devices = []
        self.serial_master_socket = None
        self.serial_slave_socket = None
        self.selected_master_device = None
        self.selected_slave_device = None
        self.last_master_cmd_sent = "none"
        self.last_slave_cmd_sent = "none"

    def config(self, serial_slave_echo: queue.Queue, serial_master_echo: queue.Queue):
        self.serial_slave_echo = serial_slave_echo
        self.serial_master_echo = serial_master_echo

    def update_available_devices(self):
        """ Update the list of available devices

        Returns:
            list: available_devices
        """
        self.available_devices = list(serial.tools.list_ports.comports())
        return self.available_devices

    def get_available_devices(self):
        """Return available devices

        Returns:
            list: available_devices
        """
        return self.available_devices

    def get_available_devices_by_name(self):
        """Return the name of every available devices

        Returns:
            list: device names
        """
        return list([i.device for i in self.available_devices])

    def get_device_name(self, device):
        """Given a device name it returnes its descriptor attribute
        based on available devices

        Args:
            device (string): device name

        Returns:
            str: device descriptor
        """
        # associate device object using device (str) as search key
        for i in self.available_devices:
            if i.device == device:
                return i.description

    def set_selected_device(self, device, device_type):
        """Set selected master or slave device

        Args:
            device (string): device name
            device_type (string): "master" or "slave"
        """
        # device is a string
        if device_type == "master":
            self.selected_master_device = device
        elif device_type == "slave":
            self.selected_slave_device = device

    def get_selected_device(self, device_type):
        """Get selected master or slave device

        Args:
            device_type (string): "master" or "slave"

        Returns:
            device: selected device
        """
        if device_type == "master":
            return self.selected_master_device
        elif device_type == "slave":
            return self.selected_slave_device

    def connect_to_device(self, device_name, device_type):
        """Open serial connection to dialog with a device

         Args:
            device_name (string): device name
            device_type (string): "master" or "slave"

        Returns:
            bool: True if connection is successful, False otherwise
        """
        try:
            if device_type == "master":
                self.serial_master_socket = serial.Serial(
                    device_name, 115200, timeout=1)
                self.selected_master_device = device_name
                return self.serial_master_socket.is_open
            elif device_type == "slave":
                self.serial_slave_socket = serial.Serial(
                    device_name, 115200, timeout=1)
                self.selected_slave_device = device_name
                return self.serial_slave_socket.is_open
        except Exception as e:
            return False

    def close_connection(self, device_type):
        reti = True
        try:
            if device_type == "master" and self.serial_master_socket.is_open:
                self.serial_master_socket.close()
            elif device_type == "slave" and self.serial_slave_socket.is_open:
                self.serial_slave_socket.close()
        except Exception as e:
            reti = False
        return reti

    def send_hello(self, device_name, device_type):
        """Send hello message to the device

         Args:
            device_name (string): device name
            device_type (string): "master" or "slave"
        """
        if device_type == "master" and (self.serial_master_socket is not None or self.serial_master_socket.is_open):
            self.serial_master_socket.write(
                ("HELLO FROM DEAR PY GUI \r\n").encode('utf-8'))
        elif device_type == "slave" and (self.serial_slave_socket is not None or self.serial_slave_socket.is_open):
            self.serial_slave_socket.write(
                ("HELLO FROM DEAR PY GUI \r\n").encode('utf-8'))
        else:
            print("ERROR: Device not connected!")

    def send_cmd(self, cmd, device_type):
        """Send custom command to the device

        Args:
            device_name (str): device name
            cmd (str): command to send
            device_type (str): "master" or "slave"
        """
        if device_type == "master" and (self.serial_master_socket is not None or self.serial_master_socket.is_open):
            self.serial_master_socket.write((cmd + "\r").encode('utf-8'))
        elif device_type == "slave" and (self.serial_slave_socket is not None or self.serial_slave_socket.is_open):
            self.serial_slave_socket.write((cmd+"\r\n").encode('utf-8'))
        else:
            print("ERROR: Device not connected!")

    def serialize_payload(self, payload, device_type):
        packet = bytearray()
        packet.extend(pack("<"+("f"*len(payload)), *payload))
        print(packet)
        if device_type == "master" and (self.serial_master_socket is not None or self.serial_master_socket.is_open):
            self.serial_master_socket.write(packet)
            self.serial_master_socket.write("\n".encode('utf-8'))
        elif device_type == "slave" and (self.serial_slave_socket is not None or self.serial_slave_socket.is_open):
            self.serial_slave_socket.write(packet)
            self.serial_slave_socket.write("\n".encode('utf-8'))

        # print(packet)
        # print(len(packet))
        # for i in packet:
        #     print(i)

        # x = unpack(">"+("f"*len(payload)), packet)
        # print(x)

    def custom_serialize_payload(self, payload, device_type, configuration):
        packet = bytearray()
        packet.extend(pack("<"+configuration, *payload))
        print(packet)
        if device_type == "master" and (self.serial_master_socket is not None or self.serial_master_socket.is_open):
            self.serial_master_socket.write(packet)
            self.serial_master_socket.write("\n".encode('utf-8'))
        elif device_type == "slave" and (self.serial_slave_socket is not None or self.serial_slave_socket.is_open):
            self.serial_slave_socket.write(packet)
            self.serial_slave_socket.write("\n".encode('utf-8'))

    def read_line(self, device_type) -> bytes:
        try:
            if device_type == "master" and (self.serial_master_socket is not None or self.serial_master_socket.is_open):
                return self.serial_master_socket.readline()
            elif device_type == "slave" and (self.serial_slave_socket is not None or self.serial_slave_socket.is_open):
                return self.serial_slave_socket.readline()
        except Exception as e:
            pass
            #print(f"ERROR {e} - {device_type} device not connected!")

    def slave_echo_callback(self):
        while self.is_running:
            slave_tx = self.read_line("slave")
            if self.read_line("slave") != None:
                # Note that with a fixed size buffer all NULL chars are filed with \x00
                # To display in a correct way on the gui it's necessary to erase those chars
                slave_tx = slave_tx.decode('utf-8').strip().strip('\x00')
                self.serial_slave_echo.put(slave_tx)
            time.sleep(0.05)

    def master_echo_callback(self):
        while self.is_running:
            master_tx = self.read_line("master")
            print(f"MASTER CALLBACK {master_tx}")
            if self.read_line("master") != None:
                # Note that with a fixed size buffer all NULL chars are filed with \x00
                # To display in a correct way on the gui it's necessary to erase those chars
                master_tx = master_tx.decode('utf-8').strip().strip('\x00')
                self.serial_master_echo.put(master_tx)
            time.sleep(0.05)

    def run(self):
        self.is_running = True
        self.serial_slave_listener = threading.Thread(
            target=self.slave_echo_callback)
        self.serial_slave_listener.start()
        self.serial_master_listener = threading.Thread(
            target=self.master_echo_callback)
        self.serial_master_listener.start()

    def stop(self):
        self.running = False
        self.serial_slave_listener.join()
        self.serial_master_listener.join()
