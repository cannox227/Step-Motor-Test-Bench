/*
 * Torque_Task.c
 *
 *  Created on: Mar 9, 2022
 *      Author: prom.tecnici
 */


#include "Torque_Task.h"

#include <string.h>
#include <stdio.h>
#include "app_threadx.h"

TX_THREAD *self;
uint16_t adc_digital, adc_values[SAMPLES];
float adc_volt, trq_volt, trq, coeff;
char msg[MSG_LEN], msg0 [MSG_LEN], msg1 [MSG_LEN];
float temp = 0.0;

void task_Torque(uint32_t interval){
	self = _tx_thread_identify();
	while(1){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		//adc_digital = HAL_ADC_GetValue(&hadc1);
		temp = 0.0;
		for(uint8_t i=0; i<SAMPLES; i++){
			adc_values[i] =  HAL_ADC_GetValue(&hadc1);
			temp += adc_values[i];
			//	  	  		 sprintf(msg1, "%ADC_VALUE: %d\r\n", adc_values[i]);
			//	  	  		 HAL_UART_Transmit(&huart2, (uint8_t*)msg1, strlen(msg1), 200);
		}
		adc_digital = temp / SAMPLES;
		sprintf(msg, "ADC_DIGITAL %d\r\n", adc_digital);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 200);

		adc_volt = MAX_GPIO_VOLTAGE / ADC_BIT * adc_digital; //Convert to voltage
		trq_volt = adc_volt / GAIN_FILTER; //Convert to .5-4.5V sensor range
		coeff = 1 / SLOPE_TORQUE_SENSOR * 1000; // [Nm/V] coefficient of torque sensor
		trq = coeff * trq_volt; //Convert to Nm value
		sprintf(msg0, "TRQ %.2f \r\n", trq);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg0, strlen(msg0), 200);
		HAL_UART_Transmit(&huart2, (uint8_t*)"\n\r", strlen("\n\r"), 200);
		tx_thread_sleep(interval);
	}

}

void getSamples(){
	 HAL_ADC_Start(&hadc1);
	 HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	 for(uint8_t i=0; i<SAMPLES; i++){
		 //HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		 adc_values[i] =  HAL_ADC_GetValue(&hadc1);
		 temp += adc_values[i];
		 sprintf(msg, "%hu\r\n", adc_values[i]);
		 	 HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	 }
	 adc_digital = temp / SAMPLES; //mean value
}

void getRealTorque(){
	adc_volt = MAX_GPIO_VOLTAGE / ADC_BIT * adc_digital; //Convert to voltage

	trq_volt = adc_volt / GAIN_FILTER; //Convert to .5-4.5V sensor range

	coeff = 1 / SLOPE_TORQUE_SENSOR * 1000; // [Nm/V] coefficient of torque sensor
	trq = coeff * trq_volt; //Convert to Nm value
}
