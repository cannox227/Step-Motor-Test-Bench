/*
 * Torque_Task.h
 *
 *  Created on: Mar 9, 2022
 *      Author: prom.tecnici
 */

#ifndef TORQUE_TORQUE_TASK_H_
#define TORQUE_TORQUE_TASK_H_


#include <stdint.h>
#include "main.h"
#include "app_threadx.h"
#include "adc.h"
#include "usart.h"

#define ADC_BIT 4096.0
#define GAIN_FILTER 0.73 //filter and conditioning block
#define SLOPE_TORQUE_SENSOR 781.43 //[mV/Nm]
#define SAMPLES 30
#define MAX_GPIO_VOLTAGE 3.3
#define MSG_LEN 30

void task_Torque();
void getSamples();
void getRealTorque();

#endif /* TORQUE_TORQUE_TASK_H_ */
