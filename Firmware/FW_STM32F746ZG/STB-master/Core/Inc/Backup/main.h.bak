/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern float brake_single_step_period;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_7
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define STEPPER_VOLTAGE_ADC_Pin GPIO_PIN_3
#define STEPPER_VOLTAGE_ADC_GPIO_Port GPIOC
#define TORQUE_ADC_Pin GPIO_PIN_3
#define TORQUE_ADC_GPIO_Port GPIOA
#define CURRENT_ADC_Pin GPIO_PIN_5
#define CURRENT_ADC_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define DEBUG_SIGNAL_2_Pin GPIO_PIN_0
#define DEBUG_SIGNAL_2_GPIO_Port GPIOG
#define DEBUG_PIN_5_Pin GPIO_PIN_10
#define DEBUG_PIN_5_GPIO_Port GPIOB
#define DEBUG_PIN_4_Pin GPIO_PIN_11
#define DEBUG_PIN_4_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define PWM_BRAKE_Pin GPIO_PIN_8
#define PWM_BRAKE_GPIO_Port GPIOC
#define ENABLE_STEPPER_SUPPLY_Pin GPIO_PIN_9
#define ENABLE_STEPPER_SUPPLY_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define DEBUG_SIGNAL_1_Pin GPIO_PIN_0
#define DEBUG_SIGNAL_1_GPIO_Port GPIOD
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define DEBUG_PIN_3_Pin GPIO_PIN_0
#define DEBUG_PIN_3_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define UART_USB                          huart3
#define UART_SLAVE                        huart6
#define TIMER_BRAKE_PWM                   htim3
#define TIMER_BRAKE_PWM_CHANNEL           TIM_CHANNEL_3
#define TIMER_ADC_TRIGGER_CONVERSIONS     htim1
#define TIMER_ADC_CHANNEL_CURRENT_TRIGGER TIM_CHANNEL_1
#define TIMER_ADC_CHANNEL_TORQUE_TRIGGER  TIM_CHANNEL_2
#define TIMER_ADC_CHANNEL_VOLTAGE_TRIGGER TIM_CHANNEL_3
#define TIMER_BRAKE_STEP_CONTROL          htim6
#define ADC_CURRENT                       hadc2
#define ADC_TORQUE                        hadc1
#define ADC_VOLTAGE                       hadc3
#define DMA_CURRENT                       hdma_adc1
#define DMA_TORQUE                        hdma_adc2
#define DMA_VOLTAGE                       hdma_adc3

// #define TIMER_BRAKE_STEP_CONTROL                htim2
// #define TIMER_BRAKE_STEP_CONTROL_CHANNEL        TIM_CHANNEL_1
// #define TIMER_BRAKE_STEP_CONTROL_ACTIVE_CHANNEL HAL_TIM_ACTIVE_CHANNEL_1

#define MAX_STEPPER_SUPPLY 24
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
