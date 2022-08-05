/*
 * serial_to_GUI_Task.c
 *
 *  Created on: Aug 2, 2022
 *      Author: macbookpro
 */

#include "serial_cmd_parser.h"

TX_THREAD *self_gui;
static TX_QUEUE *ptr_motor_queue = NULL;

void task_Serial_to_GUI_Init(TX_QUEUE *motor_queue){
	ptr_motor_queue = motor_queue;
	STG_handler.is_cmd_received = false;
	memset((uint8_t *)STG_handler.cmd_received, 0x0, sizeof(char)*RX_BUFF_SIZE);
	STG_handler.msg.cmd = NONE;
	STG_handler.msg.val = 0;
}


void task_Serial_to_GUI(uint32_t interval){
	self_gui = _tx_thread_identify(); // TODO capirne il senso
	while(1){
		HAL_UART_Receive_IT(&UART_TO_GUI, (uint8_t *)STG_handler.cmd_received, RX_BUFF_SIZE);
		if(STG_handler.is_cmd_received){
			bool parse_error = false;
			build_cmd(&STG_handler);
			parse_error = parse_cmd(ptr_motor_queue, &STG_handler);
			uint8_t bp = 0;
			if(!parse_error){
				bp = 1;
			} else {
				bp = 2;
			}
			memset((uint8_t *)STG_handler.cmd_received, 0x0, sizeof(char)*RX_BUFF_SIZE);
			STG_handler.is_cmd_received = false;
		}
	}
}

