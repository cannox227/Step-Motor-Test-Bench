/*
 * UART_Task.c
 *
 *  Created on: Feb 11, 2022
 *      Author: prom.tecnici
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "app_threadx.h"
#include "UART_Task.h"

TX_THREAD *self;
//static task_state self_state = TASK_S_NULL;
char tx_buff[TX_BUFF_SIZE];
uint8_t tx_buff_size;
char rx_buff[RX_BUFF_SIZE];
char msg_buff[250];
char motor_cmd[MOTOR_CMD_SIZE+1];
char cmd_params[MOTOR_CMD_PARAM_SIZE+1];
__IO ITStatus UartReady0 = RESET;
static TX_QUEUE *ptr_motor_queue = NULL;
static TX_QUEUE *ptr_uart_cmd_queue = NULL;
uint8_t received = 0;
motor_cmd_msg msg;
uart_cmd_msg uart_msg;

uint8_t options[] = "Usage: cmd + steps:\n\r"
		"Important note: The number length must be equal to 6, fill with zero if it's shorter!\n\r"
		" MOVE FW: fw+123456\n\r"
		" MOVE BW: bw+123456\n\r"
		" HOLD: hd+000000\n\r"
		" STOP: st+000000\n\r"
		" GET POS: ps+000000\n\r"
		" GO TO: gt+123456\n\r"
		" GO HOME: gh+000000\n\r"
		;

void task_UART(uint32_t interval){
	self = _tx_thread_identify();
	memset((uint8_t *)rx_buff, 0x0, sizeof(char)*RX_BUFF_SIZE);
	memset((uint8_t*)tx_buff, (uint8_t)0x0, sizeof(char)*TX_BUFF_SIZE);
	while(1){
		tx_queue_receive(ptr_uart_cmd_queue,&uart_msg,TX_WAIT_FOREVER);
		if(uart_msg.cmd == MOTOR_GET_POS){
			sprintf(msg_buff, "Actual motor position %lu \r\n", uart_msg.pos);
			HAL_UART_Transmit(&huart2, (uint8_t*)msg_buff, strlen(msg_buff), 100);
		}else{
			if(received == 1){
				memset((uint8_t *) rx_buff, 0x0, sizeof(char)*RX_BUFF_SIZE);
				HAL_UART_Receive_IT(&huart2, (uint8_t *) rx_buff, RX_BUFF_SIZE);
				received = 0;
			}
			else{
				tx_thread_sleep(interval);
				// hard fix?
			}
		}


	}

	//	memset((uint8_t *) rx_buff, 0x0, sizeof(char)*RX_BUFF_SIZE);
	//	memset((uint8_t*)tx_buff, (uint8_t)0x0, sizeof(char)*TX_BUFF_SIZE);
	//	while(1){
	//		if(received == 1){
	//			memset((uint8_t *) rx_buff, 0x0, sizeof(char)*RX_BUFF_SIZE);
	//			HAL_UART_Receive_IT(&huart2, (uint8_t *) rx_buff, RX_BUFF_SIZE);
	//			received = 0;
	//		}else{
	//			tx_thread_sleep(interval);
	//		}
	//	}
}

void task_UART_Init(TX_QUEUE *motor_queue, TX_QUEUE *uart_queue){
	ptr_motor_queue = motor_queue;
	ptr_uart_cmd_queue = uart_queue;
	tx_buff_size = (uint8_t) sprintf((char *) tx_buff, "Ready\n\r");
	HAL_UART_Transmit(&huart2, (uint8_t*)tx_buff, tx_buff_size, 100);
	HAL_UART_Transmit(&huart2, (uint8_t*)options, strlen((const char *)options), 100);
	HAL_UART_Receive_IT(&huart2, (uint8_t *)rx_buff, RX_BUFF_SIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart2)
{
	ULONG retui = 0;
	received = 1;
	build_cmd();
	retui = parse_cmd();
	if (retui != TX_SUCCESS)
	{
		sprintf(tx_buff, "[echo]%s\n\r[retui]%lu\n\r", rx_buff, retui);
	}
	else
	{
		sprintf(tx_buff, " [echo] %s \n\r",rx_buff);
	}
	if(HAL_UART_Transmit_IT(huart2, (uint8_t*)tx_buff, strlen(tx_buff))!= HAL_OK)
	{
		Error_Handler();
	}
	memset((uint8_t *) rx_buff, 0x0, sizeof(char)*RX_BUFF_SIZE);
	//memset((uint8_t*)tx_buff, (uint8_t)0x0, sizeof(char)*TX_BUFF_SIZE);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart2)
{
	//Error_Handler();
	HAL_UART_DeInit(huart2);
	HAL_Delay(50);
	HAL_UART_Init(huart2);
	HAL_Delay(50);
	tx_buff_size = (uint8_t) sprintf((char *) tx_buff, "-- UART RESET --\n\r");
	if(HAL_UART_Transmit_IT(huart2, (uint8_t*)tx_buff, tx_buff_size)!= HAL_OK)
	{
		Error_Handler();
	}
}

void build_cmd(){
	strncpy(motor_cmd, rx_buff, 2);
	motor_cmd[2] = '\0';
	strncpy(cmd_params, &rx_buff[3], 6);
	cmd_params[6] = '\0';
}

ULONG parse_cmd(){
	ULONG ret_val = 0;
	if(strcmp(motor_cmd, "fw")==0){
		msg.cmd = MOTOR_MOVE_FW;
		msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "bw")==0)){
		msg.cmd = MOTOR_MOVE_FW;
		msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "hd")==0)){
		msg.cmd = MOTOR_HOLD;
		msg.val = 0;
	}else if((strcmp(motor_cmd, "st")==0)){
		msg.cmd = MOTOR_STOP;
		msg.val = 0;
	}else if(strcmp(motor_cmd, "ps")==0){
		msg.cmd = MOTOR_GET_POS;
		msg.val = 0;
	}else if((strcmp(motor_cmd, "gt")==0)){
		msg.cmd = MOTOR_GO_TO;
		msg.val = atoi(cmd_params);
	}else if((strcmp(motor_cmd, "gh")==0)){
		msg.cmd = MOTOR_GO_HOME;
		msg.val = 0;
	}else{
		ret_val = 1;
		//HAL_UART_Transmit(&huart2, (uint8_t*)"Wrong command!\r\n", strlen("Wrong command!\r\n"),100);
	}
	if(ret_val == 0){
		tx_queue_send(ptr_motor_queue, &msg, TX_NO_WAIT);
	}
	return ret_val;
}
