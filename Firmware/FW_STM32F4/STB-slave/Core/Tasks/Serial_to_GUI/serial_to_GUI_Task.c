/*
 * serial_to_GUI_Task.c
 *
 *  Created on: Aug 2, 2022
 *      Author: macbookpro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "app_threadx.h"
#include "serial_to_GUI_Task.h"
#include <stdbool.h>

TX_THREAD *self;

char tx_buff[TX_BUFF_SIZE];
char motor_cmd[MOTOR_CMD_SIZE+1];
char cmd_params[MOTOR_CMD_PARAM_SIZE+1];
//TODO motor cmd msg

typedef struct{
	bool is_cmd_received;
	char cmd_received[RX_BUFF_SIZE];
	motor_cmd_msg msg;
} serial_to_GUI_handler;

serial_to_GUI_handler STG_handler;

void task_Serial_to_GUI_Init(){
	STG_handler.is_cmd_received = false;
	memset((uint8_t *)STG_handler.cmd_received, 0x0, sizeof(char)*RX_BUFF_SIZE);
	STG_handler.msg.cmd = NONE;
	STG_handler.msg.val = 0;
}

// TODO: decide if keep this function here or in a separate file shared between the two serial tasks
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == UART_TO_GUI.Instance){
		STG_handler.is_cmd_received = true;
	}

}

void task_Serial_to_GUI(uint32_t interval){
	self = _tx_thread_identify(); // TODO capirne il senso
	while(1){
		HAL_UART_Receive_IT(&UART_TO_GUI, (uint8_t *)STG_handler.cmd_received, RX_BUFF_SIZE);
		if(STG_handler.is_cmd_received){
			bool parse_error = false;
			build_cmd();
			parse_error = parse_cmd();
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

void build_cmd(){
	strncpy(motor_cmd, STG_handler.cmd_received, MOTOR_CMD_SIZE);
	//	motor_cmd[MOTOR_CMD_SIZE] = '\0';
	strncpy(cmd_params, &STG_handler.cmd_received[MOTOR_CMD_SIZE+1], MOTOR_CMD_PARAM_SIZE);
	//	STG_handler.cmd_received[MOTOR_CMD_PARAM_SIZE] = '\0';
}

bool parse_cmd(){
	bool ret_val = false;
	if(strcmp(motor_cmd, "fw")==0){
		STG_handler.msg.cmd = MOTOR_MOVE_FW;
		STG_handler.msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "bw")==0)){
		STG_handler.msg.cmd = MOTOR_MOVE_FW;
		STG_handler.msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "hd")==0)){
		STG_handler.msg.cmd = MOTOR_HOLD;
		STG_handler.msg.val = 0;
	}else if((strcmp(motor_cmd, "st")==0)){
		STG_handler.msg.cmd = MOTOR_STOP;
		STG_handler.msg.val = 0;
	}else if(strcmp(motor_cmd, "ps")==0){
		STG_handler.msg.cmd = MOTOR_GET_POS;
		STG_handler.msg.val = 0;
	}else if((strcmp(motor_cmd, "gt")==0)){
		STG_handler.msg.cmd = MOTOR_GO_TO;
		STG_handler.msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "gh")==0)){
		STG_handler.msg.cmd = MOTOR_GO_HOME;
		STG_handler.msg.val = 0;
	}else{
		ret_val = true;
	}

	return ret_val;
}
