import json

# variable where config params are stored
configuration = {}

def read_config():
    global configuration
    with open("motor_params.json", "r") as read_file:
        configuration = json.load(read_file)
        print(f"Config file: \n {configuration}")

def write_config():
    with open("motor_params.json", "w") as write_file:
        json.dump(configuration, write_file, indent=2)

def set_value(id, val):
    configuration[id] = val

def get_value(id):
    return configuration[id]