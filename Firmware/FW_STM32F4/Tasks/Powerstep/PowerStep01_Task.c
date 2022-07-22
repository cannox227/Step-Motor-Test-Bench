#ifdef OLD
/*
 * PowerStep_Task.c
 *
 *  Created on: Nov 22, 2021
 *      Author: maurizio.rossi
 */
#include "main.h"
#include "app_threadx.h"
#include "Powerstep01_Task.h"

#define CURRENT_MODE

static task_state self_state = TASK_S_NULL;
static TX_QUEUE *ptr_motor_queue = NULL;
motor_cmd_msg msg;
static volatile uint16_t gLastError;

static void BusyInterruptHandler(void);
static void FlagInterruptHandler(void);
static void ErrorHandler(uint16_t error);

void task_Powerstep01(uint32_t interval)
{
  //self_state = TASK_S_INIT;
  initMotor();
  //goForward(5000);
  while(1)
  {
	//goForward(5000);
    switch(self_state)
    {
      case TASK_S_INIT:
      {
        /* check current state! */
        //FlagInterruptHandler();
        break;
      }
      case TASK_S_RUN:
      {
        tx_queue_receive(ptr_motor_queue,&msg,TX_WAIT_FOREVER);
        switch(msg.cmd)
        {
          case MOTOR_STOP:
          {
            BSP_MotorControl_CmdHardHiZ(0);
            break;
          }
          case MOTOR_HOLD:
          {
            BSP_MotorControl_HardStop(0);
            break;
          }
          case MOTOR_MOVE_FW:
          {
        	goForward(msg.val);
            //BSP_MotorControl_Move(0, FORWARD , msg.val);
            break;
          }
          case MOTOR_MOVE_BW:
          {
            goBackward(msg.val);
        	//BSP_MotorControl_Move(0, BACKWARD , msg.val);
            break;
          }
          case MOTOR_GET_POS:
          {
        	  //HAL_UART_Transmit(huart, pData, Size, Timeout)
        	  break;
          }
          case MOTOR_GO_TO:
          {
        	  goTo(msg.val);
        	  break;
          }
          case MOTOR_GO_HOME:
          {
               goHome();
               break;
           }

          default:
          {
            break;
          }
        }

        break;
      }
      default:
      {
        break;
      }
    }
    tx_thread_sleep(interval);
  }

}

void task_Powerstep01_Init(TX_QUEUE *queue)
{
  ptr_motor_queue = queue;
  self_state = TASK_S_RUN;
}
void initMotor()
{
	#ifdef CURRENT_MODE
	/* Initialization parameters for current mode */
	union powerstep01_Init_u initDeviceParameters =
	{
	  /* common parameters */
	  .cm.cp.cmVmSelection = POWERSTEP01_CM_VM_CURRENT, // enum powerstep01_CmVm_t
	  582, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
	  582, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
	  488, // Maximum speed in step/s, range 15.25 to 15610 steps/s
	  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
	  POWERSTEP01_LSPD_OPT_ON, // Low speed optimization bit, enum powerstep01_LspdOpt_t
	  244.16, // Full step speed in step/s, range 7.63 to 15625 steps/s
	  POWERSTEP01_BOOST_MODE_OFF, // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
	  281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
	  STEP_MODE_HALF, // Step mode settings via enum motorStepMode_t
	  POWERSTEP01_SYNC_SEL_DISABLED, // Synch. Mode settings via enum powerstep01_SyncSel_t
	  (POWERSTEP01_ALARM_EN_OVERCURRENT|
	   POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN|
	   POWERSTEP01_ALARM_EN_THERMAL_WARNING|
	   POWERSTEP01_ALARM_EN_UVLO|
	   POWERSTEP01_ALARM_EN_STALL_DETECTION|
	   POWERSTEP01_ALARM_EN_SW_TURN_ON|
	   POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
	  POWERSTEP01_IGATE_64mA, // Gate sink/source current via enum powerstep01_Igate_t
	  POWERSTEP01_TBOOST_0ns, // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
	  POWERSTEP01_TCC_500ns, // Controlled current time via enum powerstep01_Tcc_t
	  POWERSTEP01_WD_EN_DISABLE, // External clock watchdog, enum powerstep01_WdEn_t
	  POWERSTEP01_TBLANK_375ns, // Duration of the blanking time via enum powerstep01_TBlank_t
	  POWERSTEP01_TDT_125ns, // Duration of the dead time via enum powerstep01_Tdt_t
	  /* current mode parameters */
	  20.0,//200.0, //328.12, // Hold torque in mV, range from 7.8mV to 1000 mV
	  20.0,//200.0, //328.12, // Running torque in mV, range from 7.8mV to 1000 mV
	  20.0,//200.0, //328.12, // Acceleration torque in mV, range from 7.8mV to 1000 mV
	  20.0,//200.0, //328.12, // Deceleration torque in mV, range from 7.8mV to 1000 mV
	  POWERSTEP01_TOFF_FAST_8us, //Maximum fast decay time , enum powerstep01_ToffFast_t
	  POWERSTEP01_FAST_STEP_12us, //Maximum fall step time , enum powerstep01_FastStep_t
	  3.0, // Minimum on-time in us, range 0.5us to 64us
	  21.0, // Minimum off-time in us, range 0.5us to 64us
	  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
	  POWERSTEP01_CONFIG_SW_HARD_STOP, // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
	  POWERSTEP01_CONFIG_TQ_REG_TVAL_USED, // External torque regulation enabling , enum powerstep01_ConfigEnTqReg_t
	  POWERSTEP01_CONFIG_VS_COMP_DISABLE, // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t
	  POWERSTEP01_CONFIG_OC_SD_DISABLE, // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
	  POWERSTEP01_CONFIG_UVLOVAL_LOW, // UVLO Threshold via powerstep01_ConfigUvLoVal_t
	  POWERSTEP01_CONFIG_VCCVAL_15V, // VCC Val, enum powerstep01_ConfigVccVal_t
	  POWERSTEP01_CONFIG_TSW_048us, // Switching period, enum powerstep01_ConfigTsw_t
	  POWERSTEP01_CONFIG_PRED_DISABLE, // Predictive current enabling , enum powerstep01_ConfigPredEn_t
	};
	#endif //CURRENT_MODE

	#ifdef VOLTAGE_MODE
	/* Initialization parameters for voltage mode */
	union powerstep01_Init_u initDeviceParameters =
	{
	  /* common parameters */
	  .vm.cp.cmVmSelection = POWERSTEP01_CM_VM_VOLTAGE, // enum powerstep01_CmVm_t
	  500, //5000, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
	  500, //5000, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
	  1000,//2000, // Maximum speed in step/s, range 15.25 to 15610 steps/s
	  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
	  POWERSTEP01_LSPD_OPT_OFF, // Low speed optimization bit, enum powerstep01_LspdOpt_t
	  2000, // Full step speed in step/s, range 7.63 to 15625 steps/s
	  POWERSTEP01_BOOST_MODE_ON, // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
	  312.5, //281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
	  STEP_MODE_HALF, //STEP_MODE_1_16, // Step mode settings via enum motorStepMode_t
	  POWERSTEP01_SYNC_SEL_DISABLED, // Synch. Mode settings via enum powerstep01_SyncSel_t
	  (POWERSTEP01_ALARM_EN_OVERCURRENT|
	   POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN|
	   POWERSTEP01_ALARM_EN_THERMAL_WARNING|
	   POWERSTEP01_ALARM_EN_UVLO|
	   POWERSTEP01_ALARM_EN_STALL_DETECTION|
	   POWERSTEP01_ALARM_EN_SW_TURN_ON|
	   POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
	  POWERSTEP01_IGATE_64mA, // Gate sink/source current via enum powerstep01_Igate_t
	  POWERSTEP01_TBOOST_1000ns, //POWERSTEP01_TBOOST_0ns, // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
	  POWERSTEP01_TCC_1000ns, //POWERSTEP01_TCC_500ns, // Controlled current time via enum powerstep01_Tcc_t
	  POWERSTEP01_WD_EN_DISABLE, // External clock watchdog, enum powerstep01_WdEn_t
	  POWERSTEP01_TBLANK_1000ns, //POWERSTEP01_TBLANK_375ns, // Duration of the blanking time via enum powerstep01_TBlank_t
	  POWERSTEP01_TDT_1000ns, //POWERSTEP01_TDT_125ns, // Duration of the dead time via enum powerstep01_Tdt_t
	  /* voltage mode parameters */
	  5.0, //16.02, // Hold duty cycle (torque) in %, range 0 to 99.6%
	  5.0, //16.02, // Run duty cycle (torque) in %, range 0 to 99.6%
	  5.0, //16.02, // Acceleration duty cycle (torque) in %, range 0 to 99.6%
	  5.0, //16.02, // Deceleration duty cycle (torque) in %, range 0 to 99.6%
	  240,//2000, //61.512, // Intersect speed settings for BEMF compensation in steps/s, range 0 to 3906 steps/s
	  0.08, //0.03815, // BEMF start slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
	  0.03, //0.06256, // BEMF final acc slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
	  0.03, //0.06256, // BEMF final dec slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
	  1, // Thermal compensation param, range 1 to 1.46875
	  531.25, // Stall threshold settings in mV, range 31.25mV to 1000mV
	  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
	  POWERSTEP01_CONFIG_SW_HARD_STOP, // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
	  POWERSTEP01_CONFIG_VS_COMP_DISABLE, // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t
	  POWERSTEP01_CONFIG_OC_SD_DISABLE, // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
	  POWERSTEP01_CONFIG_UVLOVAL_HIGH, //POWERSTEP01_CONFIG_UVLOVAL_LOW, // UVLO Threshold via powerstep01_ConfigUvLoVal_t
	  POWERSTEP01_CONFIG_VCCVAL_15V, // VCC Val, enum powerstep01_ConfigVccVal_t
	  POWERSTEP01_CONFIG_PWM_DIV_2, // PWM Frequency Integer division, enum powerstep01_ConfigFPwmInt_t
	  POWERSTEP01_CONFIG_PWM_MUL_1, // PWM Frequency Integer Multiplier, enum powerstep01_ConfigFPwmDec_t
	};
	#endif //VOLTAGE_MODE

	/* Set the Powerstep01 library to use 1 device */
	BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 1);
	/* When BSP_MotorControl_Init is called with NULL pointer,                  */
    /* the Powerstep01 registers are set with the predefined values from file   */
	/* powerstep01_target_config.h, otherwise the registers are set using the   */
	/* powerstep01_Init_u relevant union of structure values.                   */
	/* The first call to BSP_MotorControl_Init initializes the first device     */
    /* whose Id is 0.                                                           */
	/* The nth call to BSP_MotorControl_Init initializes the nth device         */
	/* whose Id is n-1.                                                         */
	/* Uncomment the call to BSP_MotorControl_Init below to initialize the      */
	/* device with the union declared in the the main.c file and comment the    */
	/* subsequent call having the NULL pointer                                  */
	//BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, &initDeviceParameters);
	BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
	/* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
	BSP_MotorControl_AttachFlagInterrupt(FlagInterruptHandler);
	/* Attach the function MyBusyInterruptHandler (defined below) to the busy interrupt */
	BSP_MotorControl_AttachBusyInterrupt(BusyInterruptHandler);
    /* Attach the function Error_Handler (defined below) to the error Handler*/
	BSP_MotorControl_AttachErrorHandler(ErrorHandler);
}
/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
static void FlagInterruptHandler(void)
{
//  /* Get the value of the status register via the command GET_STATUS */
//  uint16_t statusRegister = BSP_MotorControl_CmdGetStatus(0);
//
//  /* Check HIZ flag: if set, power brigdes are disabled */
//  if ((statusRegister & POWERSTEP01_STATUS_HIZ) == POWERSTEP01_STATUS_HIZ)
//  {
//    // HIZ state
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "HiZ State\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check BUSY flag: if not set, a command is under execution */
//  if ((statusRegister & POWERSTEP01_STATUS_BUSY) == 0)
//  {
//    // BUSY
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Busy\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check SW_F flag: if not set, the SW input is opened */
//  if ((statusRegister & POWERSTEP01_STATUS_SW_F ) == 0)
//  {
//    // SW OPEN
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "SW Open\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  else
//  {
//    // SW CLOSED
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "SW Closed\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  /* Check SW_EN bit */
//  if ((statusRegister & POWERSTEP01_STATUS_SW_EVN) == POWERSTEP01_STATUS_SW_EVN)
//  {
//    // switch turn_on event
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Turn ON event\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  /* Check direction bit */
//  if ((statusRegister & POWERSTEP01_STATUS_DIR) == 0)
//  {
//    // BACKWARD
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Backward\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  else
//  {
//    // FORWARD
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Forward\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) == POWERSTEP01_STATUS_MOT_STATUS_STOPPED )
//  {
//    // MOTOR STOPPED
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Motor Stopped\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) == POWERSTEP01_STATUS_MOT_STATUS_ACCELERATION )
//  {
//    // MOTOR ACCELERATION
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Motor Acceleration\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) == POWERSTEP01_STATUS_MOT_STATUS_DECELERATION )
//  {
//    // MOTOR DECELERATION
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Motor Deceleration\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) == POWERSTEP01_STATUS_MOT_STATUS_CONST_SPD )
//  {
//    // MOTOR RUNNING AT CONSTANT SPEED
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Constant speed\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check Command Error flag: if set, the command received by SPI can't be */
//  /* performed. This occurs for instance when a move command is sent to the */
//  /* Powerstep01 while it is already running */
//  if ((statusRegister & POWERSTEP01_STATUS_CMD_ERROR) == POWERSTEP01_STATUS_CMD_ERROR)
//  {
//    // Command Error
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Command Error\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check Step mode clock flag: if set, the device is working in step clock mode */
//  if ((statusRegister & POWERSTEP01_STATUS_STCK_MOD) == POWERSTEP01_STATUS_STCK_MOD)
//  {
//    //Step clock mode enabled
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Step Clock mode enabled\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check UVLO flag: if not set, there is an undervoltage lock-out */
//  if ((statusRegister & POWERSTEP01_STATUS_UVLO) == 0)
//  {
//    //undervoltage lock-out
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Undervoltage lock-out\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check UVLO ADC flag: if not set, there is an ADC undervoltage lock-out */
//  if ((statusRegister & POWERSTEP01_STATUS_UVLO_ADC) == 0)
//  {
//    //ADC undervoltage lock-out
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "ADC undervoltage lock-out\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check thermal STATUS flags: if  set, the thermal status is not normal */
//  if ((statusRegister & POWERSTEP01_STATUS_TH_STATUS) != 0)
//  {
//    //thermal status: 1: Warning, 2: Bridge shutdown, 3: Device shutdown
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Thermal status: %d\n\r", (statusRegister & POWERSTEP01_STATUS_TH_STATUS));
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check OCD  flag: if not set, there is an overcurrent detection */
//  if ((statusRegister & POWERSTEP01_STATUS_OCD) == 0)
//  {
//    //overcurrent detection
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Overcurrent Detection\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check STALL_A flag: if not set, there is a Stall condition on bridge A */
//  if ((statusRegister & POWERSTEP01_STATUS_STALL_A) == 0)
//  {
//    //overcurrent detection
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Stall on bridge A\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }
//
//  /* Check STALL_B flag: if not set, there is a Stall condition on bridge B */
//  if ((statusRegister & POWERSTEP01_STATUS_STALL_B) == 0)
//  {
//    //overcurrent detection
//    uart_buff_size = (uint8_t) sprintf((char *) uart_buff, "Stall on bridge B\n\r");
//    HAL_UART_Transmit(&huart3, uart_buff, uart_buff_size, 5000);
//  }

}

/**
  * @brief  This function is the User handler for the busy interrupt
  * @param  None
  * @retval None
  */
static void BusyInterruptHandler(void)
{

   if (BSP_MotorControl_CheckBusyHw())
   {
      /* Busy pin is low, so at list one Powerstep01 chip is busy */
     /* To be customized (for example Switch on a LED) */
   }
   else
   {
     /* To be customized (for example Switch off a LED) */
   }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param[in] error Number of the error
  * @retval None
  */
static void ErrorHandler(uint16_t error)
{
  /* Backup error number */
  gLastError = error;

  /* Infinite loop */
  while(1)
  {
  }
}

void goForward(uint32_t N_STEP){
	BSP_MotorControl_Move(0, FORWARD, N_STEP);
	BSP_MotorControl_WaitWhileActive(0);
}

void goBackward(uint32_t N_STEP){
	BSP_MotorControl_Move(0, BACKWARD, N_STEP);
	BSP_MotorControl_WaitWhileActive(0);
}

void goTo(int32_t POSITION){
	BSP_MotorControl_GoTo(0, POSITION);
	BSP_MotorControl_WaitWhileActive(0);
}

void goHome(){
	BSP_MotorControl_GoHome(0);
	BSP_MotorControl_WaitWhileActive(0);
}


int32_t getPosition(){
	return BSP_MotorControl_GetPosition(0);
}


#endif
