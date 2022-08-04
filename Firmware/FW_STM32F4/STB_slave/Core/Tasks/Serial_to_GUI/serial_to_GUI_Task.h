/*
 * serial_to_GUI_Task.h
 *
 *  Created on: Aug 2, 2022
 *      Author: macbookpro
 */

#ifndef SERIAL_TO_GUI_TASK_H_
#define SERIAL_TO_GUI_TASK_H_

#include "main.h"
#include "app_threadx.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

#define MOTOR_CMD_SIZE 2
#define MOTOR_CMD_PARAM_SIZE 6
#define MOTOR_CMD_PADDING 3
#define RX_BUFF_SIZE (MOTOR_CMD_SIZE+ MOTOR_CMD_PARAM_SIZE+MOTOR_CMD_PADDING) //11
#define TX_BUFF_SIZE 300


void task_Serial_to_GUI(uint32_t interval);
void task_Serial_to_GUI_Init(TX_QUEUE *motor_queue); //TX_QUEUE *uart_queue,
void task_Serial_to_GUI_Init();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart3);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart2);
void build_cmd();
bool parse_cmd();

#endif /* SERIAL_TO_GUI_TASK_H_ */
