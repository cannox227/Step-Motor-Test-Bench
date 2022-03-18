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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "x_nucleo_ihmxx.h"
#include "powerstep01.h"
#include "x_nucleo_ihm03a1_stm32f4xx.h"
#include "Powerstep_Task.h"
#include "UART_Task.h"
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define BSP_MOTOR_CONTROL_BOARD_TIMER_STEP_CLOCK_Pin GPIO_PIN_7
#define BSP_MOTOR_CONTROL_BOARD_TIMER_STEP_CLOCK_GPIO_Port GPIOC
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_Pin GPIO_PIN_9
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_GPIO_Port GPIOA
#define BSP_MOTOR_CONTROL_BOARD_FLAG_Pin GPIO_PIN_10
#define BSP_MOTOR_CONTROL_BOARD_FLAG_GPIO_Port GPIOA
#define BSP_MOTOR_CONTROL_BOARD_FLAG_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define BSP_MOTOR_CONTROL_BOARD_BUSY_Pin GPIO_PIN_5
#define BSP_MOTOR_CONTROL_BOARD_BUSY_GPIO_Port GPIOB
#define BSP_MOTOR_CONTROL_BOARD_BUSY_EXTI_IRQn EXTI9_5_IRQn
#define BSP_MOTOR_CONTROL_BOARD_CS_Pin GPIO_PIN_6
#define BSP_MOTOR_CONTROL_BOARD_CS_GPIO_Port GPIOB
#define PWM_BRAKE_Pin GPIO_PIN_9
#define PWM_BRAKE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
