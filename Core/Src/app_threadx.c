/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <stdio.h>
#include "UART_Task.h"
#include "Powerstep_Task.h"
#include "Torque_Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t stack_GreenLED_task[THREAD_STACK_SIZE];
TX_THREAD ptr_GreenLED_task;
uint8_t stack_Powerstep01_task[THREAD_STACK_SIZE];
TX_THREAD ptr_Powerstep01_task;
uint8_t stack_Serial_task[THREAD_STACK_SIZE];
TX_THREAD ptr_Serial_task;
uint8_t stack_Torque_task[THREAD_STACK_SIZE];
TX_THREAD ptr_Torque_task;
motor_cmd_msg stack_motor_cmd_queue[QUEUE_STACK_SIZE];
TX_QUEUE ptr_motor_cmd_queue;
uart_cmd_msg stack_uart_cmd_queue[QUEUE_STACK_SIZE];
TX_QUEUE ptr_uart_cmd_queue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void task_GreenLED(uint32_t initial_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
  /* Tasks creation */

//    tx_thread_create(&ptr_GreenLED_task, "task_GreenLED",
//  		  task_GreenLED, 25,
//  		  stack_GreenLED_task, THREAD_STACK_SIZE, 10, 10, 1,
//  		  TX_AUTO_START);


    tx_thread_create(&ptr_Powerstep01_task, "task_Powerstep",
  		  task_Powerstep01, 30,
  		  stack_Powerstep01_task, THREAD_STACK_SIZE, 7, 7, 1,
  		  TX_AUTO_START);

    tx_thread_create(&ptr_Serial_task, "task_UART",
                task_UART, 10,
                stack_Serial_task, THREAD_STACK_SIZE, 9, 9, 1,
                TX_AUTO_START);


//    tx_thread_create(&ptr_Torque_task, "task_Torque",
//                task_Torque, 2000,
//                stack_Torque_task, THREAD_STACK_SIZE, 8, 8, 1,
//                TX_AUTO_START);


    /* Queue creation */
    tx_queue_create(&ptr_motor_cmd_queue, "motor command queue", 1, stack_motor_cmd_queue, QUEUE_STACK_SIZE);
    tx_queue_create(&ptr_uart_cmd_queue, "uart command queue",1, stack_uart_cmd_queue, QUEUE_STACK_SIZE);
    /* tasks initialization */

    task_UART_Init(&ptr_motor_cmd_queue, &ptr_uart_cmd_queue);
    task_Powerstep01_Init(&ptr_motor_cmd_queue, &ptr_uart_cmd_queue);
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void task_GreenLED(uint32_t interval)
{
  while(1)
  {
    //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  //initMotor();
	  //goForward(100000);
	uint8_t x = 0;
	printf("%i",x);
    tx_thread_sleep(interval);
  }
}


/* USER CODE END 1 */
