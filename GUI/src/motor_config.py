import json


class Motor_Config():
    def __init__(self):
        self.last_filename = ""
        self.configuration = {}
        self.current_path = "configs/motors_params.json"

    def init_config(self, conf_name):
        obj = '''
        {
            "config_name": "configuration_01",
            "max_speed": 0,
            "min_speed": 0,
            "full_step_speed": 0,
            "overcurrent_threshold": 0,
            "step_mode": "A",
            "working_mode": "Voltage mode",
            "current_hold_torque_mv": 0,
            "current_running_torque_mv": 0,
            "current_acceleration_torque_mv": 0,
            "current_deceleration_torque_mv": 0,
            "current_min_on_time_us": 0,
            "current_min_off_time_us": 0,
            "voltage_hold_perc": 0,
            "voltage_run_perc": 0,
            "voltage_acc_perc": 0,
            "voltage_dec_perc": 0,
            "voltage_min_on_time_us": 0,
            "voltage_min_off_time_us": 0,
            "bemf_start_slope": 0,
            "bemf_final_acc_slope": 0,
            "bemf_final_dec_slope": 0,
            "thermal_compensation": 0,
            "stall_threshold": 0
        }
        '''
        obj = json.loads(obj)
        obj['config_name'] = conf_name
        self.configuration = obj
        print(self.configuration)

    # Get local configuration file
    def get_config(self):
        return self.configuration

    # Write configuration file as json object
    def write_config_file(self, src):
        with open(str(src), "w") as write_file:
            json.dump(self.configuration, write_file, indent=2)

    # Read existing configuration file and load it as json object
    def read_config_file(self, src):
        with open(str(src), "r") as read_file:
            self.configuration = json.load(read_file)
            print(f"Config file: \n {self.configuration}")

    # Set value of a specific field in configuration
    def set_value(self, id, val):
        self.configuration[id] = val

    # Get value of a specific field in configuration
    def get_value(self, id):
        return self.configuration[id]

    def get_last_filename(self):
        return self.last_filename

    def set_last_filename(self, filename):
        self.last_filename = filename
