#ifdef OLD
/*
 * Powerstep_Task.h
 *
 *  Created on: Nov 22, 2021
 *      Author: maurizio.rossi
 */

#ifndef POWERSTEP_POWERSTEP01_TASK_H_
#define POWERSTEP_POWERSTEP01_TASK_H_

#include <stdint.h>
#include "app_threadx.h"

void task_Powerstep01(uint32_t interval);
void task_Powerstep01_Init(TX_QUEUE *queue);
void initMotor(void);
void goForward(uint32_t N_STEP);
void goBackward(uint32_t N_STEP);
void goTo(int32_t POSITION);
void goHome();
int32_t getPosition();
#endif /* POWERSTEP_POWERSTEP_TASK_H_ */
#endif
