#ifdef CURRENT_MODE
/* Initialization parameters for current mode */
union powerstep01_Init_u initDeviceParameters =
    {
        /* common parameters */
        .cm.cp.cmVmSelection = POWERSTEP01_CM_VM_CURRENT, // enum powerstep01_CmVm_t
        582,                                              // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
        582,                                              // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
        488,                                              // Maximum speed in step/s, range 15.25 to 15610 steps/s
        0,                                                // Minimum speed in step/s, range 0 to 976.3 steps/s
        POWERSTEP01_LSPD_OPT_ON,                          // Low speed optimization bit, enum powerstep01_LspdOpt_t
        244.16,                                           // Full step speed in step/s, range 7.63 to 15625 steps/s
        POWERSTEP01_BOOST_MODE_OFF,                       // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
        281.25,                                           // Overcurrent threshold settings via enum powerstep01_OcdTh_t
        STEP_MODE_HALF,                                   // Step mode settings via enum motorStepMode_t
        POWERSTEP01_SYNC_SEL_DISABLED,                    // Synch. Mode settings via enum powerstep01_SyncSel_t
        (POWERSTEP01_ALARM_EN_OVERCURRENT |
         POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN |
         POWERSTEP01_ALARM_EN_THERMAL_WARNING |
         POWERSTEP01_ALARM_EN_UVLO |
         POWERSTEP01_ALARM_EN_STALL_DETECTION |
         POWERSTEP01_ALARM_EN_SW_TURN_ON |
         POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
        POWERSTEP01_IGATE_64mA,                 // Gate sink/source current via enum powerstep01_Igate_t
        POWERSTEP01_TBOOST_0ns,                 // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
        POWERSTEP01_TCC_500ns,                  // Controlled current time via enum powerstep01_Tcc_t
        POWERSTEP01_WD_EN_DISABLE,              // External clock watchdog, enum powerstep01_WdEn_t
        POWERSTEP01_TBLANK_375ns,               // Duration of the blanking time via enum powerstep01_TBlank_t
        POWERSTEP01_TDT_125ns,                  // Duration of the dead time via enum powerstep01_Tdt_t
        /* current mode parameters */
        20.0,                                     // 200.0, //328.12, // Hold torque in mV, range from 7.8mV to 1000 mV
        20.0,                                     // 200.0, //328.12, // Running torque in mV, range from 7.8mV to 1000 mV
        20.0,                                     // 200.0, //328.12, // Acceleration torque in mV, range from 7.8mV to 1000 mV
        20.0,                                     // 200.0, //328.12, // Deceleration torque in mV, range from 7.8mV to 1000 mV
        POWERSTEP01_TOFF_FAST_8us,                // Maximum fast decay time , enum powerstep01_ToffFast_t
        POWERSTEP01_FAST_STEP_12us,               // Maximum fall step time , enum powerstep01_FastStep_t
        3.0,                                      // Minimum on-time in us, range 0.5us to 64us
        21.0,                                     // Minimum off-time in us, range 0.5us to 64us
        POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
        POWERSTEP01_CONFIG_SW_HARD_STOP,          // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
        POWERSTEP01_CONFIG_TQ_REG_TVAL_USED,      // External torque regulation enabling , enum powerstep01_ConfigEnTqReg_t
        POWERSTEP01_CONFIG_VS_COMP_DISABLE,       // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t
        POWERSTEP01_CONFIG_OC_SD_DISABLE,         // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
        POWERSTEP01_CONFIG_UVLOVAL_LOW,           // UVLO Threshold via powerstep01_ConfigUvLoVal_t
        POWERSTEP01_CONFIG_VCCVAL_15V,            // VCC Val, enum powerstep01_ConfigVccVal_t
        POWERSTEP01_CONFIG_TSW_048us,             // Switching period, enum powerstep01_ConfigTsw_t
        POWERSTEP01_CONFIG_PRED_DISABLE,          // Predictive current enabling , enum powerstep01_ConfigPredEn_t
};
#endif // CURRENT_MODE

#ifdef VOLTAGE_MODE
/* Initialization parameters for voltage mode */
union powerstep01_Init_u initDeviceParameters =
    {
        /* common parameters */
        .vm.cp.cmVmSelection = POWERSTEP01_CM_VM_VOLTAGE, // enum powerstep01_CmVm_t
        500,                                              // 5000, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
        500,                                              // 5000, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
        1000,                                             // 2000, // Maximum speed in step/s, range 15.25 to 15610 steps/s
        0,                                                // Minimum speed in step/s, range 0 to 976.3 steps/s
        POWERSTEP01_LSPD_OPT_OFF,                         // Low speed optimization bit, enum powerstep01_LspdOpt_t
        2000,                                             // Full step speed in step/s, range 7.63 to 15625 steps/s
        POWERSTEP01_BOOST_MODE_ON,                        // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
        312.5,                                            // 281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
        STEP_MODE_HALF,                                   // STEP_MODE_1_16, // Step mode settings via enum motorStepMode_t
        POWERSTEP01_SYNC_SEL_DISABLED,                    // Synch. Mode settings via enum powerstep01_SyncSel_t
        (POWERSTEP01_ALARM_EN_OVERCURRENT |
         POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN |
         POWERSTEP01_ALARM_EN_THERMAL_WARNING |
         POWERSTEP01_ALARM_EN_UVLO |
         POWERSTEP01_ALARM_EN_STALL_DETECTION |
         POWERSTEP01_ALARM_EN_SW_TURN_ON |
         POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
        POWERSTEP01_IGATE_64mA,                 // Gate sink/source current via enum powerstep01_Igate_t
        POWERSTEP01_TBOOST_1000ns,              // POWERSTEP01_TBOOST_0ns, // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
        POWERSTEP01_TCC_1000ns,                 // POWERSTEP01_TCC_500ns, // Controlled current time via enum powerstep01_Tcc_t
        POWERSTEP01_WD_EN_DISABLE,              // External clock watchdog, enum powerstep01_WdEn_t
        POWERSTEP01_TBLANK_1000ns,              // POWERSTEP01_TBLANK_375ns, // Duration of the blanking time via enum powerstep01_TBlank_t
        POWERSTEP01_TDT_1000ns,                 // POWERSTEP01_TDT_125ns, // Duration of the dead time via enum powerstep01_Tdt_t
        /* voltage mode parameters */
        5.0,                                      // 16.02, // Hold duty cycle (torque) in %, range 0 to 99.6%
        5.0,                                      // 16.02, // Run duty cycle (torque) in %, range 0 to 99.6%
        5.0,                                      // 16.02, // Acceleration duty cycle (torque) in %, range 0 to 99.6%
        5.0,                                      // 16.02, // Deceleration duty cycle (torque) in %, range 0 to 99.6%
        240,                                      // 2000, //61.512, // Intersect speed settings for BEMF compensation in steps/s, range 0 to 3906 steps/s
        0.08,                                     // 0.03815, // BEMF start slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
        0.03,                                     // 0.06256, // BEMF final acc slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
        0.03,                                     // 0.06256, // BEMF final dec slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
        1,                                        // Thermal compensation param, range 1 to 1.46875
        531.25,                                   // Stall threshold settings in mV, range 31.25mV to 1000mV
        POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
        POWERSTEP01_CONFIG_SW_HARD_STOP,          // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
        POWERSTEP01_CONFIG_VS_COMP_DISABLE,       // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t
        POWERSTEP01_CONFIG_OC_SD_DISABLE,         // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
        POWERSTEP01_CONFIG_UVLOVAL_HIGH,          // POWERSTEP01_CONFIG_UVLOVAL_LOW, // UVLO Threshold via powerstep01_ConfigUvLoVal_t
        POWERSTEP01_CONFIG_VCCVAL_15V,            // VCC Val, enum powerstep01_ConfigVccVal_t
        POWERSTEP01_CONFIG_PWM_DIV_2,             // PWM Frequency Integer division, enum powerstep01_ConfigFPwmInt_t
        POWERSTEP01_CONFIG_PWM_MUL_1,             // PWM Frequency Integer Multiplier, enum powerstep01_ConfigFPwmDec_t
};
