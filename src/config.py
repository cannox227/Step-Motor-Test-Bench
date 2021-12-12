import json

# variable where config params are stored
configuration = {}
current_path = "configs/motors_params.json"

def init_config(cf_name):
    global configuration
    obj = '''
    {
        "config_name": "none",
        "hold_dt": 0.0,
        "run_dt": 0.0,
        "acc": 0.0,
        "dec": 0.0,
        "step": "Half Step",
        "rotation": "Clockwise"
    }
    '''
    obj = json.loads(obj)
    obj['config_name'] = cf_name
    configuration = obj
    print(configuration)

def read_config(src):
    global configuration
    with open(str(src), "r") as read_file:
        configuration = json.load(read_file)
        print(f"Config file: \n {configuration}")

def write_config(src):
    with open(str(src), "w") as write_file:
        json.dump(configuration, write_file, indent=2)

def set_value(id, val):
    configuration[id] = val

def get_value(id):
    return configuration[id]