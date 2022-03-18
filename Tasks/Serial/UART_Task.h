/*
 * UART_Task.c
 *
 *  Created on: Feb 11, 2022
 *      Author: prom.tecnici
 */
#ifndef SERIAL_UART_TASK_H_
#define SERIAL_UARTL_TASK_H_

#include <stdint.h>
#include "main.h"
#include "app_threadx.h"
#include "usart.h"

#define RX_BUFF_SIZE 10
#define TX_BUFF_SIZE 300
#define MOTOR_CMD_SIZE 2
#define MOTOR_CMD_PARAM_SIZE 6


void task_UART(uint32_t interval);
void task_UART_Init(TX_QUEUE *motor_queue, TX_QUEUE *uart_queue);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart3);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart2);
void build_cmd();
ULONG parse_cmd();

#endif
