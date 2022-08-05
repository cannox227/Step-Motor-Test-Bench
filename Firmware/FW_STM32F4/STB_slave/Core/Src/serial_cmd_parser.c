/*
 * serial_cmd_parser.c
 *
 *  Created on: Aug 5, 2022
 *      Author: macbookpro
 */

#include "serial_cmd_parser.h"
#include "app_threadx.h"
#include "serial_to_master_Task.h"
#include "serial_to_GUI_Task.h"

char tx_buff[TX_BUFF_SIZE];
char motor_cmd[MOTOR_CMD_SIZE+1];
char cmd_params[MOTOR_CMD_PARAM_SIZE+1];

serial_handler STM_handler;
serial_handler STG_handler;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == UART_TO_GUI.Instance){
		STG_handler.is_cmd_received = true;
	}else if(huart->Instance == UART_TO_MASTER.Instance){
		STM_handler.is_cmd_received = true;
	}
}

void build_cmd(serial_handler* handler){
	strncpy(motor_cmd, handler->cmd_received, MOTOR_CMD_SIZE);
	//	motor_cmd[MOTOR_CMD_SIZE] = '\0';
	strncpy(cmd_params, &handler->cmd_received[MOTOR_CMD_SIZE+1], MOTOR_CMD_PARAM_SIZE);
	//	STM_handler.cmd_received[MOTOR_CMD_PARAM_SIZE] = '\0';
}

bool parse_cmd(TX_QUEUE *ptr_motor_queue, serial_handler* handler){
	bool parse_error = false;
	if(strcmp(motor_cmd, "fw")==0){
		handler->msg.cmd = MOTOR_MOVE_FW;
		handler->msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "bw")==0)){
		handler->msg.cmd = MOTOR_MOVE_BW;
		handler->msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "hd")==0)){
		handler->msg.cmd = MOTOR_HOLD;
		handler->msg.val = 0;
	}else if((strcmp(motor_cmd, "st")==0)){
		handler->msg.cmd = MOTOR_STOP;
		handler->msg.val = 0;
	}else if(strcmp(motor_cmd, "ps")==0){
		handler->msg.cmd = MOTOR_GET_POS;
		handler->msg.val = 0;
	}else if((strcmp(motor_cmd, "gt")==0)){
		handler->msg.cmd = MOTOR_GO_TO;
		handler->msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "gh")==0)){
		handler->msg.cmd = MOTOR_GO_HOME;
		handler->msg.val = 0;
	}else{
		parse_error = true;
	}
	if(parse_error == 0){
		tx_queue_send(ptr_motor_queue, &handler->msg, TX_NO_WAIT);
	}
	return parse_error;
}
