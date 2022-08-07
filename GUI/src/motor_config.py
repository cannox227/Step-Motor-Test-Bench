import json


class Motor_Config():
    def __init__(self):
        self.last_filename = ""
        self.configuration = {}
        self.current_path = "configs/motors_params.json"

        POWERSTEP01_IGATE_SHIFT = 5
        self.powerstep01_Igate_t = {
            "POWERSTEP01_IGATE_4mA": bin(0 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_4mA_Bis": bin(1 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_8mA": bin(2 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_16mA": bin(3 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_24mA": bin(4 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_32mA": bin(5 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_64mA": bin(6 << POWERSTEP01_IGATE_SHIFT),
            "POWERSTEP01_IGATE_96mA": bin(7 << POWERSTEP01_IGATE_SHIFT),
        }

        self.motorStepMode_t = {
            "STEP_MODE_FULL": 0,
            "STEP_MODE_HALF": 1,
            "STEP_MODE_1_4 ": 2,
            "STEP_MODE_1_8 ": 3,
            "STEP_MODE_1_16": 4,
            "STEP_MODE_1_32": 5,
            "STEP_MODE_1_64": 6,
            "STEP_MODE_1_128": 7,
            "STEP_MODE_1_256": 8,
            "STEP_MODE_UNKNOW": 254,
            "STEP_MODE_WAVE": 255
        }

        self.powerstep01_LspdOpt_t = {
            "POWERSTEP01_LSPD_OPT_OFF": 0,
            "POWERSTEP01_LSPD_OPT_ON": 4096
        }

        self.powerstep01_BoostMode_t = {
            "POWERSTEP01_BOOST_MODE": 0,
            "POWERSTEP01_FS_SPD_MASK": 8192
        }

        POWERSTEP01_TBOOST_SHIFT = 8
        self.powerstep01_Tboost_t = {
            "POWERSTEP01_TBOOST_0ns": bin(0 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_62_5__83_3__125ns": bin(1 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_125ns": bin(2 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_250ns": bin(3 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_375ns": bin(4 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_500ns": bin(5 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_750ns": bin(6 << POWERSTEP01_TBOOST_SHIFT),
            "POWERSTEP01_TBOOST_1000ns": bin(7 << POWERSTEP01_TBOOST_SHIFT),
        }

        POWERSTEP01_TCC_SHIFT = 0
        self.powerstep01_Tcc_t = {
            "POWERSTEP01_TCC_125ns": bin(0x00 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_250ns": bin(0x01 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_375ns": bin(0x02 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_500ns": bin(0x03 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_625ns": bin(0x04 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_750ns": bin(0x05 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_875ns": bin(0x06 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1000ns": bin(0x07 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1125ns": bin(0x08 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1250ns": bin(0x09 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1375ns": bin(0x0A << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1500ns": bin(0x0B << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1625ns": bin(0x0C << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1750ns": bin(0x0D << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_1875ns": bin(0x0E << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2000ns": bin(0x0F << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2125ns": bin(0x10 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2250ns": bin(0x11 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2375ns": bin(0x12 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2500ns": bin(0x13 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2625ns": bin(0x14 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2750ns": bin(0x15 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_2875ns": bin(0x16 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3000ns": bin(0x17 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3125ns": bin(0x18 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3250ns": bin(0x19 << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3375ns": bin(0x1A << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3500ns": bin(0x1B << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3625ns": bin(0x1C << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3750ns": bin(0x1D << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3750ns_bis": bin(0x1E << POWERSTEP01_TCC_SHIFT),
            "POWERSTEP01_TCC_3750ns_ter": bin(0x1F << POWERSTEP01_TCC_SHIFT)
        }

        self.powerstep01_WdEn_t = {
            "POWERSTEP01_WD_EN_DISABLE": bin(0),
            "POWERSTEP01_WD_EN_ENABLE": bin(1 << 11)
        }

        POWERSTEP01_TBLANK_SHIFT = 5
        self.powerstep01_TBlank_t = {
            "POWERSTEP01_TBLANK_125ns": bin(0x00 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_250ns": bin(0x01 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_375ns": bin(0x02 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_500ns": bin(0x03 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_625ns": bin(0x04 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_750ns": bin(0x05 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_875ns": bin(0x06 << POWERSTEP01_TBLANK_SHIFT),
            "POWERSTEP01_TBLANK_1000ns": bin(0x07 << POWERSTEP01_TBLANK_SHIFT),
        }
        POWERSTEP01_TDT_SHIFT = 0
        self.powerstep01_Tdt_t = {
            "POWERSTEP01_TDT_125ns": bin(0x00 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_250ns": bin(0x01 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_375ns": bin(0x02 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_500ns": bin(0x03 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_625ns": bin(0x04 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_750ns": bin(0x05 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_875ns": bin(0x06 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1000ns": bin(0x07 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1125ns": bin(0x08 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1250ns": bin(0x09 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1375ns": bin(0x0A << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1500ns": bin(0x0B << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1625ns": bin(0x0C << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1750ns": bin(0x0D << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_1875ns": bin(0x0E << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2000ns": bin(0x0F << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2125ns": bin(0x10 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2250ns": bin(0x11 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2375ns": bin(0x12 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2500ns": bin(0x13 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2625ns": bin(0x14 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2750ns": bin(0x15 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_2875ns": bin(0x16 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3000ns": bin(0x17 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3125ns": bin(0x18 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3250ns": bin(0x19 << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3375ns": bin(0x1A << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3500ns": bin(0x1B << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3625ns": bin(0x1C << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3750ns": bin(0x1D << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_3875ns": bin(0x1E << POWERSTEP01_TDT_SHIFT),
            "POWERSTEP01_TDT_4000ns": bin(0x1F << POWERSTEP01_TDT_SHIFT)
        }

        self.powerstep01_CmVm_t = {
            "POWERSTEP01_CM_VM_VOLTAGE": (0x00),
            "POWERSTEP01_CM_VM_CURRENT": (0x08)
        }

        # current structs
        POWERSTEP01_TOFF_FAST_SHIFT = 4
        self.powerstep01_ToffFast_t = {
            "POWERSTEP01_TOFF_FAST_2us": bin(0x00 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_4us": bin(0x01 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_6us": bin(0x02 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_8us": bin(0x03 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_10us": bin(0x04 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_12us": bin(0x05 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_14us": bin(0x06 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_16us": bin(0x07 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_18us": bin(0x08 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_20us": bin(0x09 << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_22us": bin(0x0A << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_24us": bin(0x0B << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_26us": bin(0x0C << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_28us": bin(0x0D << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_30us": bin(0x0E << POWERSTEP01_TOFF_FAST_SHIFT),
            "POWERSTEP01_TOFF_FAST_32us": bin(0x0F << POWERSTEP01_TOFF_FAST_SHIFT)
        }
        POWERSTEP01_FAST_STEP_SHIFT = 0
        self.powerstep01_FastStep_t = {
            "POWERSTEP01_FAST_STEP_2us":  bin(0x00 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_4us":  bin(0x01 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_6us":  bin(0x02 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_8us":  bin(0x03 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_10us": bin(0x04 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_12us": bin(0x05 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_14us": bin(0x06 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_16us": bin(0x07 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_18us": bin(0x08 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_20us": bin(0x09 << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_22us": bin(0x0A << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_24s": bin(0x0B << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_26us": bin(0x0C << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_28us": bin(0x0D << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_30us": bin(0x0E << POWERSTEP01_FAST_STEP_SHIFT),
            "POWERSTEP01_FAST_STEP_32us": bin(0x0F << POWERSTEP01_FAST_STEP_SHIFT)
        }

    def init_config(self, conf_name):
        obj = '''
        {
            "config_name": "configuration_01",
            "acceleration_rate": 0,
            "deceleration_rate": 0,
            "max_speed": 0,
            "min_speed": 0,
            "low_speed_optimization_bit": 0,
            "full_step_speed": 0,
            "boost_mode_bit": 0,
            "overcurrent_threshold": 0,
            "step_mode": "A",
            "gate_sink_source_current": 0,
            "turn_off_time_boost": 0,
            "tcc": 0,
            "clock_wd_enable": 0,
            "blanking_time": 0,
            "dead_time": 0,
            "working_mode": "Voltage mode",
            "current_hold_torque_mv": 0,
            "current_running_torque_mv": 0,
            "current_acceleration_torque_mv": 0,
            "current_deceleration_torque_mv": 0,
            "current_max_decay_time_us": 0,
            "current_max_fall_time_us":0,
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

    def get_STEP_MODE_keys(self):
        return list(self.motorStepMode_t.keys())

    def get_STEP_mode(self, step_mode):
        # uint8_t
        return self.motorStepMode_t[step_mode]

    def get_LSPD_bit_keys(self):
        return list(self.powerstep01_LspdOpt_t.keys())

    def get_LSPD_bit(self, LSPD):
        # uint16_t
        return self.powerstep01_LspdOpt_t[LSPD]

    def get_BOOST_mode_keys(self):
        return list(self.powerstep01_BoostMode_t.keys())

    def get_BOOST_mode(self, BOOST_mode):
        # uint16_t
        return self.powerstep01_BoostMode_t[BOOST_mode]

    def get_I_GATE_keys(self):
        return list(self.powerstep01_Igate_t.keys())

    def get_I_GATE(self, I_GATE):
        return self.powerstep01_Igate_t[I_GATE]

    def get_TBOOST_keys(self):
        return list(self.powerstep01_Tboost_t.keys())

    def get_TBOOST(self, TBOOST):
        return self.powerstep01_Tboost_t[TBOOST]

    def get_TCC_keys(self):
        return list(self.powerstep01_Tcc_t.keys())

    def get_TCC(self, TCC):
        return self.powerstep01_Tcc_t[TCC]

    def get_WD_EN_keys(self):
        # uint16_t
        return list(self.powerstep01_WdEn_t.keys())

    def get_WD_EN(self, WD_EN):
        return self.powerstep01_WdEn_t[WD_EN]

    def get_TBLANK_keys(self):
        return list(self.powerstep01_TBlank_t.keys())

    def get_TBLANK(self, TBLANK):
        return self.powerstep01_TBlank_t[TBLANK]

    def get_TDT_keys(self):
        return list(self.powerstep01_Tdt_t.keys())

    def get_TDT(self, TDT):
        return self.powerstep01_Tdt_t[TDT]

    def get_CM_VM_keys(self):
        return list(self.powerstep01_CmVm_t.keys())

    def get_CM_VM(self, CM_VM):
        return self.powerstep01_CmVm_t[CM_VM]

    def get_TOFF_FAST_keys(self):
        return list(self.powerstep01_ToffFast_t.keys())

    def get_TOFF_FAST(self, TOFF_FAST):
        return self.powerstep01_ToffFast_t[TOFF_FAST]

    def get_FAST_STEP_keys(self):
        return list(self.powerstep01_FastStep_t.keys())

    def get_FAST_STEP(self, FAST_STEP):
        return self.powerstep01_FastStep_t[FAST_STEP]

    def get_value_by_key(self, key, current_val):
        if key == "low_speed_optimization_bit":
            return self.get_LSPD_bit(current_val)
        elif key == "boost_mode_bit":
            return self.get_BOOST_mode(current_val)
        elif key == "gate_sink_source_current":
            return self.get_I_GATE(current_val)
        elif key == "turn_off_time_boost":
            return self.get_TBOOST(current_val)
        elif key == "tcc":
            return self.get_TCC(current_val)
        elif key == "clock_wd_enable":
            return self.get_WD_EN(current_val)
        elif key == "blanking_time":
            return self.get_TBLANK(current_val)
        elif key == "dead_time":
            return self.get_TDT(current_val)
        elif key == "step_mode":
            return self.get_STEP_mode(current_val)
        elif key == "working_mode":
            return self.get_CM_VM(current_val)
        elif key == "current_max_decay_time_us":
            return self.get_TOFF_FAST(current_val)
        elif key == "current_max_fall_time_us":
            return self.get_FAST_STEP(current_val)
        else:
            return -1
