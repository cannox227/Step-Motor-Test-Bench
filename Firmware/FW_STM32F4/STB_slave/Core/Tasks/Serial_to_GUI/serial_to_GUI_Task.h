/*
 * serial_to_GUI_Task.h
 *
 *  Created on: Aug 2, 2022
 *      Author: macbookpro
 */

#ifndef SERIAL_TO_GUI_TASK_H_
#define SERIAL_TO_GUI_TASK_H_

#include "serial_cmd_parser.h"

void task_Serial_to_GUI(uint32_t interval);
void task_Serial_to_GUI_Init(TX_QUEUE *motor_queue); //TX_QUEUE *uart_queue,

#endif /* SERIAL_TO_GUI_TASK_H_ */
