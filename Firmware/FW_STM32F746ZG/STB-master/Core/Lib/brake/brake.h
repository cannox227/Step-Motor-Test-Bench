#ifndef BRAKE_H__
#define BRAKE_H__
/**
 * @file brake.h
 * @author Magnetic brake utilities (tommaso.canova@studenti.unitn.it)
 * @brief
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "tim.h"

#include <stdbool.h>

#define FREQUENCY_25_KHZ_IN_MS 0.04
#define MAX_ALLOWED_PWM_DUTY   1
#define MIN_ALLOWED_PWM_DUTY   0
#define BRAKE_STEP             0.1
#define TOTAL_PWM_BRAKE_STEPS  11.0f  // 0 to 100%

typedef struct {
    float pwm_duty_cycle;
    float pwm_period;
    bool is_braking;
    bool is_top_limit_reached;
} brake_handle;

extern brake_handle magnetic_brake;

void brake_init(brake_handle *brake);
float brake_get_pwm_duty_cycle(brake_handle *brake);
void brake_set_pwm_duty_cycle(brake_handle *brake, float value);
void brake_on(brake_handle *brake);
void brake_off(brake_handle *brake);
void brake_increase_duty(brake_handle *brake, float factor);
void brake_decrease_duty(brake_handle *brake, float factor);
void brake_oc_handler(TIM_HandleTypeDef *htim);
void brake_clear_top_limit_flag(brake_handle *brake);
#endif