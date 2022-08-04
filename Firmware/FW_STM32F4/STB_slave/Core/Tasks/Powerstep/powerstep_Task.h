/*
 * powerstep.h
 *
 *  Created on: Aug 3, 2022
 *      Author: macbookpro
 */

#ifndef TASKS_POWERSTEP_POWERSTEP_TASK_H_
#define TASKS_POWERSTEP_POWERSTEP_TASK_H_

#include <stdint.h>
#include "app_threadx.h"

extern uint32_t position;

void task_Powerstep01(uint32_t interval);
void task_Powerstep01_Init(TX_QUEUE *motor_queue);
void initMotor(void);
void goForward(uint32_t N_STEP);
void goBackward(uint32_t N_STEP);
void goTo(int32_t POSITION);
void goHome();
int32_t getPosition();
#endif /* TASKS_POWERSTEP_POWERSTEP_TASK_H_ */
