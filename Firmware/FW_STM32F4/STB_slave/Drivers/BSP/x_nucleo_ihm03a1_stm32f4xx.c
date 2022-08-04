/**
  ******************************************************************************
  * @file    x_nucleo_ihm03a1_stm32f4xx.c
  * @author  IPC Rennes
  * @version V1.6.0
  * @date    January 25th, 2016
  * @brief   BSP driver for x-nucleo-ihm03a1 Nucleo extension board
  *  (based on powerSTEP01)
  ******************************************************************************
* @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "x_nucleo_ihm03a1_stm32f4xx.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup X_NUCLEO_IHM03A1_STM32F3XX X_NUCLEO_IHM03A1_STM32F3XX
  * @{
  */

/* Private constants ---------------------------------------------------------*/

/** @addtogroup IHM03A1_Private_Constants
  * @{
  */

/// Timer Prescaler
#define TIMER_PRESCALER (64)

/// SPI Maximum Timeout values for flags waiting loops
#define SPIx_TIMEOUT_MAX                      ((uint32_t)0x1000)

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/** @addtogroup IHM03A1_Board_Private_Variables
  * @{
  */
/// SPI handler declaration
extern SPI_HandleTypeDef hspi1;
/// Timer handler for step clock
extern TIM_HandleTypeDef htim4;
/**
  * @}
  */

/** @defgroup IHM03A1_Board_Private_Function_Prototypes Board Private Function Prototypes
  * @{
  */

void Powerstep01_Board_Delay(uint32_t delay);         //Delay of the requested number of milliseconds
void Powerstep01_Board_EnableIrq(void);               //Disable Irq
void Powerstep01_Board_DisableIrq(void);              //Enable Irq
void Powerstep01_Board_GpioInit(uint8_t deviceId);    //Initialise GPIOs used for a powerSTEP01 device
void Powerstep01_Board_StepClockInit(void);           //Initialise the step clock
void Powerstep01_Board_StartStepClock(uint16_t newFreq); //Start the step clock by using the given frequency
void Powerstep01_Board_StopStepClock(void);              //Stop the PWM uses for the step clock
void Powerstep01_Board_ReleaseReset(uint8_t deviceId);   //Reset the powerSTEP01 reset pin
void Powerstep01_Board_Reset(uint8_t deviceId);          //Set the powerSTEP01 reset pin
uint8_t Powerstep01_Board_SpiInit(void);   //Initialise the SPI used for powerSTEP01s
uint8_t Powerstep01_Board_SpiWriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte, uint8_t nbDevices, uint16_t size); //Write bytes to the powerSTEP01s via SPI
uint32_t Powerstep01_Board_BUSY_PIN_GetState(void); //Returns the BUSY pin state
uint32_t Powerstep01_Board_FLAG_PIN_GetState(void); //Returns the FLAG pin state

/**
  * @}
  */


/** @defgroup  IHM03A1_Board_Private_Functions Board Private Functions
  * @{
  */

/******************************************************//**
 * @brief This function provides an accurate delay in milliseconds
 * @param[in] delay  time length in milliseconds
 * @retval None
 **********************************************************/
void Powerstep01_Board_Delay(uint32_t delay)
{
  HAL_Delay(delay);
}

/******************************************************//**
 * @brief This function disable the interruptions
 * @retval None
 **********************************************************/
void Powerstep01_Board_DisableIrq(void)
{
  __disable_irq();
}

/******************************************************//**
 * @brief This function enable the interruptions
 * @retval None
 **********************************************************/
void Powerstep01_Board_EnableIrq(void)
{
  __enable_irq();
}

/******************************************************//**
 * @brief  Initiliases the GPIOs used by the powerSTEP01s
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES-1 )
 * @retval None
  **********************************************************/
void Powerstep01_Board_GpioInit(uint8_t deviceId)
{
//  //GPIO_InitTypeDef GPIO_InitStruct;
//
//  if (deviceId==0)
//  {
//    /* GPIO Ports Clock Enable */
//    __GPIOC_CLK_ENABLE();
//    __GPIOA_CLK_ENABLE();
//    __GPIOB_CLK_ENABLE();
//
////    /* Configure Powerstep01 - Busy pin --------------------------------------*/
////    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_BUSY_PIN;
////    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
////    GPIO_InitStruct.Pull = GPIO_PULLUP;
////    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
////    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_BUSY_PORT, &GPIO_InitStruct);
//
//    /* Set Priority of Exti line Interrupt used for the busy interrupt*/
//    HAL_NVIC_SetPriority(BUSY_EXTI_LINE_IRQn, 6, 0);
//
//    /* Enable the Exti line Interrupt used for the busy interrupt*/
//    HAL_NVIC_EnableIRQ(BUSY_EXTI_LINE_IRQn);
//
////    /* Configure Powerstep01 - Flag pin --------------------------------------*/
////    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_FLAG_PIN;
////    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
////    GPIO_InitStruct.Pull = GPIO_PULLUP;
////    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
////    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_FLAG_PORT, &GPIO_InitStruct);
//
//    /* Set Priority of Exti lineInterrupt used for the Flag interrupt*/
//    HAL_NVIC_SetPriority(FLAG_EXTI_LINE_IRQn, 5, 0);
//
//    /* Enable the Exti line  Interrupt used for the Flag interrupt*/
//    HAL_NVIC_EnableIRQ(FLAG_EXTI_LINE_IRQn);
//
//    /* Configure Powerstep01 - CS pin ----------------------------------------*/
////    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_CS_PIN;
////    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
////    GPIO_InitStruct.Pull = GPIO_NOPULL;
////    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
////    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_CS_PORT, &GPIO_InitStruct);
////    HAL_GPIO_WritePin(BSP_MOTOR_CONTROL_BOARD_CS_PORT, BSP_MOTOR_CONTROL_BOARD_CS_PIN, GPIO_PIN_SET);
//
//    /* Configure Powerstep01 - STBY/RESET pin --------------------------------*/
////    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN;
////    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
////    GPIO_InitStruct.Pull = GPIO_NOPULL;
////    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
////    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT, &GPIO_InitStruct);
//
//    /* Reset Powerstep0*/
//    Powerstep01_Board_Reset(deviceId);
//
//    /* Let a delay after reset */
//    Powerstep01_Board_Delay(1);
//  }

}

/******************************************************//**
 * @brief  Initialises the step clock by setting
 * corresponding GPIO, Timer, Pwm,...
 * @retval None
 **********************************************************/
void Powerstep01_Board_StepClockInit(void)
{
//  TIM_OC_InitTypeDef sConfigOC;
//  TIM_MasterConfigTypeDef sMasterConfig;
//  TIM_ClockConfigTypeDef sClockSourceConfig;
//
//  hTimStepClock.Instance = BSP_MOTOR_CONTROL_BOARD_TIMER_STEP_CLOCK;
//  hTimStepClock.Init.Prescaler = TIMER_PRESCALER -1;
//  hTimStepClock.Init.CounterMode = TIM_COUNTERMODE_UP;
//  hTimStepClock.Init.Period = 0;
//  hTimStepClock.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  HAL_TIM_PWM_Init(&hTimStepClock);
//
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  HAL_TIM_ConfigClockSource(&hTimStepClock, &sClockSourceConfig);
//
//  sConfigOC.OCMode = TIM_OCMODE_PWM1;
//  sConfigOC.Pulse = 0;
//  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//  HAL_TIM_PWM_ConfigChannel(&hTimStepClock, &sConfigOC, BSP_MOTOR_CONTROL_BOARD_CHAN_TIMER_STEP_CLOCK);
//
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  HAL_TIMEx_MasterConfigSynchronization(&hTimStepClock, &sMasterConfig);

	//does nothing
	//Powerstep01_Board_Delay(1);
}

/******************************************************//**
 * @brief  Start the step clock by using the given frequency
 * @param[in] newFreq in Hz of the step clock
 * @retval None
 * @note The frequency is directly the current speed of the device
 **********************************************************/
void Powerstep01_Board_StartStepClock(uint16_t newFreq)
{
  uint32_t sysFreq = HAL_RCC_GetSysClockFreq();
  uint32_t period = (sysFreq/ (TIMER_PRESCALER * newFreq)) - 1;

  __HAL_TIM_SetAutoreload(&htim4, period);
  __HAL_TIM_SetCompare(&htim4, BSP_MOTOR_CONTROL_BOARD_CHAN_TIMER_STEP_CLOCK, period >> 1);
  HAL_TIM_PWM_Start_IT(&htim4, BSP_MOTOR_CONTROL_BOARD_CHAN_TIMER_STEP_CLOCK);
}

/******************************************************//**
 * @brief  Stops the PWM uses for the step clock
 * @retval None
 **********************************************************/
void Powerstep01_Board_StopStepClock(void)
{
  HAL_TIM_PWM_Stop_IT(&htim4,BSP_MOTOR_CONTROL_BOARD_CHAN_TIMER_STEP_CLOCK);
}

/******************************************************//**
 * @brief  Releases the powerSTEP01 reset (pin set to High) of all devices
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES-1 )
 * @retval None
 **********************************************************/
void Powerstep01_Board_ReleaseReset(uint8_t deviceId)
{
  HAL_GPIO_WritePin(BSP_MOTOR_CONTROL_BOARD_STBY_RESET_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_STBY_RESET_Pin, GPIO_PIN_SET);
}

/******************************************************//**
 * @brief  Resets the powerSTEP01 (reset pin set to low) of all devices
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES-1 )
 * @retval None
 **********************************************************/
void Powerstep01_Board_Reset(uint8_t deviceId)
{
  HAL_GPIO_WritePin(BSP_MOTOR_CONTROL_BOARD_STBY_RESET_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_STBY_RESET_Pin, GPIO_PIN_RESET);
}

/******************************************************//**
 * @brief  Initialise the SPI used by powerSTEP01
 * @retval HAL_OK if SPI transaction is OK, HAL_KO else
 **********************************************************/
uint8_t Powerstep01_Board_SpiInit(void)
{
//  HAL_StatusTypeDef status;
//
//  /* Initialises the SPI  --------------------------------------------------*/
//  SpiHandle.Instance               = SPIx;
//  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
//  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
//  SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
//  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
//  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
//  SpiHandle.Init.CRCPolynomial     = 7;
//  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
//  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
//  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
//  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
//
//  SpiHandle.Init.Mode = SPI_MODE_MASTER;
//
//  status = HAL_SPI_Init(&SpiHandle);
//
//  return (uint8_t) status;

	//Powerstep01_Board_Delay(1);
	return HAL_OK;
}
/******************************************************//**
 * @brief  Write and read SPI byte to the powerSTEP01
 * @param[in] pByteToTransmit pointer to the byte to transmit
 * @param[in] pReceivedByte pointer to the received byte
 * @param[in] nbDevices Number of device in the SPI chain
 * @retval HAL_OK if SPI transaction is OK, HAL_KO else
 **********************************************************/
uint8_t Powerstep01_Board_SpiWriteBytes(uint8_t *pByteToTransmit, uint8_t *pReceivedByte, uint8_t nbDevices, uint16_t size)
{
  HAL_StatusTypeDef status;
  uint32_t i;
  HAL_GPIO_WritePin(BSP_MOTOR_CONTROL_BOARD_CS_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_CS_Pin, GPIO_PIN_RESET);
  for (i = 0; i < nbDevices; i++)
  {
    status = HAL_SPI_TransmitReceive(&hspi1, pByteToTransmit, pReceivedByte, 1, SPIx_TIMEOUT_MAX);
    if (status != HAL_OK)
    {
      break;
    }
    pByteToTransmit++;
    pReceivedByte++;
  }
  HAL_GPIO_WritePin(BSP_MOTOR_CONTROL_BOARD_CS_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_CS_Pin, GPIO_PIN_SET);

  return (uint8_t) status;
}

/******************************************************//**
 * @brief  Returns the BUSY pin state.
 * @retval The BUSY pin value.
 **********************************************************/
uint32_t Powerstep01_Board_BUSY_PIN_GetState(void)
{
  return HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_BUSY_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_BUSY_Pin);
}

/******************************************************//**
 * @brief  Returns the FLAG pin state.
 * @retval The FLAG pin value.
 **********************************************************/
uint32_t Powerstep01_Board_FLAG_PIN_GetState(void)
{
  return HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_FLAG_GPIO_Port, BSP_MOTOR_CONTROL_BOARD_FLAG_Pin);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
