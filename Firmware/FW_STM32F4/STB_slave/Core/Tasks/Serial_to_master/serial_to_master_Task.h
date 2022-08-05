/*
 * serial_to_master_Task.h
 *
 *  Created on: Aug 5, 2022
 *      Author: macbookpro
 */

#ifndef TASKS_SERIAL_TO_MASTER_SERIAL_TO_MASTER_TASK_H_
#define TASKS_SERIAL_TO_MASTER_SERIAL_TO_MASTER_TASK_H_

#include "serial_cmd_parser.h"

void task_Serial_to_master(uint32_t interval);
void task_Serial_to_master_Init(TX_QUEUE *motor_queue);

#endif /* TASKS_SERIAL_TO_MASTER_SERIAL_TO_MASTER_TASK_H_ */
