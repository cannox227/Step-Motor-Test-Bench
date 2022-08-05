/*
 * serial_to_master_Task.c
 *
 *  Created on: Aug 5, 2022
 *      Author: macbookpro
 */

#include "serial_cmd_parser.h"

TX_THREAD *self_master;

static TX_QUEUE *ptr_uart_queue = NULL;
static TX_QUEUE *ptr_motor_queue = NULL;

void task_Serial_to_master_Init(TX_QUEUE *motor_queue){
	//ptr_uart_queue = uart_queue;
	ptr_motor_queue = motor_queue;
	STM_handler.is_cmd_received = false;
	memset((uint8_t *)STM_handler.cmd_received, 0x0, sizeof(char)*RX_BUFF_SIZE);
	STM_handler.msg.cmd = NONE;
	STM_handler.msg.val = 0;
}


void task_Serial_to_master(uint32_t interval){
	self_master = _tx_thread_identify();
	while(1){
		HAL_UART_Receive_IT(&UART_TO_MASTER, (uint8_t *)STM_handler.cmd_received, RX_BUFF_SIZE);
		if(STM_handler.is_cmd_received){
			bool parse_error = false;
			build_cmd(&STM_handler);
			parse_error = parse_cmd(ptr_motor_queue, &STM_handler);
			if(STM_handler.msg.cmd == MOTOR_MOVE_FW){
				HAL_UART_Transmit(&UART_TO_MASTER, (uint8_t *)"start_ack\r\n", sizeof("start_ack\r\n"), 100);
			}else if(STM_handler.msg.cmd == MOTOR_STOP){
				HAL_UART_Transmit(&UART_TO_MASTER, (uint8_t *)"stop_ack\r\n", sizeof("stop_ack\r\n"), 100);
			}
			uint8_t X = 0;
			if(!parse_error){
				X = 1;
			} else {
				X = 2;
			}
			memset((uint8_t *)STM_handler.cmd_received, 0x0, sizeof(char)*RX_BUFF_SIZE);
			STM_handler.is_cmd_received = false;
		}
	}
}


