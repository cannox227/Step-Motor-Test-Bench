#include "brake.h"

#include "main.h"
#include "pwm.h"
#include "timer_utils.h"

brake_handle magnetic_brake;

void brake_init(brake_handle *brake) {
    brake->is_braking           = false;
    brake->is_top_limit_reached = false;
    brake->pwm_period           = FREQUENCY_25_KHZ_IN_MS;  // 25Khz
    pwm_set_period(&TIMER_BRAKE_PWM, FREQUENCY_25_KHZ_IN_MS);
    brake->pwm_duty_cycle = 0;
}
float brake_get_pwm_duty_cycle(brake_handle *brake) {
    return brake->pwm_duty_cycle;
}
void brake_set_pwm_duty_cycle(brake_handle *brake, float value) {
    if (value >= MAX_ALLOWED_PWM_DUTY) {
        brake->pwm_duty_cycle = MAX_ALLOWED_PWM_DUTY;
    } else if (value <= MIN_ALLOWED_PWM_DUTY) {
        brake->pwm_duty_cycle = MIN_ALLOWED_PWM_DUTY;
    } else {
        brake->pwm_duty_cycle = value;
    }
    pwm_set_duty_cicle(&TIMER_BRAKE_PWM, TIMER_BRAKE_PWM_CHANNEL, value);
}
void brake_on(brake_handle *brake) {
    pwm_start_channel(&TIMER_BRAKE_PWM, TIMER_BRAKE_PWM_CHANNEL);
    brake->is_braking = true;
}
void brake_off(brake_handle *brake) {
    pwm_stop_channel(&TIMER_BRAKE_PWM, TIMER_BRAKE_PWM_CHANNEL);
    brake->is_braking = false;
}

void brake_increase_duty(brake_handle *brake, float factor) {
    brake_get_pwm_duty_cycle(brake) + factor >= MAX_ALLOWED_PWM_DUTY
        ? brake_set_pwm_duty_cycle(brake, MAX_ALLOWED_PWM_DUTY)
        : brake_set_pwm_duty_cycle(brake, brake_get_pwm_duty_cycle(brake) + factor);
}
void brake_decrease_duty(brake_handle *brake, float factor) {
    brake_get_pwm_duty_cycle(brake) - factor <= MIN_ALLOWED_PWM_DUTY
        ? brake_set_pwm_duty_cycle(brake, MIN_ALLOWED_PWM_DUTY)
        : brake_set_pwm_duty_cycle(brake, brake_get_pwm_duty_cycle(brake) - factor);
}
void brake_clear_top_limit_flag(brake_handle *brake) {
    brake->is_top_limit_reached = false;
}
void brake_oc_handler(TIM_HandleTypeDef *htim) {
    uint32_t volatile counter = __HAL_TIM_GetCounter(htim);
    if (htim->Instance == TIMER_BRAKE_STEP_CONTROL.Instance) {
        // if (htim->Channel == TIMER_BRAKE_STEP_CONTROL_ACTIVE_CHANNEL) {
        if (brake_get_pwm_duty_cycle(&magnetic_brake) >= MAX_ALLOWED_PWM_DUTY) {
            magnetic_brake.is_top_limit_reached = true;
        } else {
            brake_increase_duty(&magnetic_brake, BRAKE_STEP);
        }
        uint8_t x = 0;
        //uint32_t volatile lil_step = TIM_MS_TO_TICKS(htim, brake_single_step_period);
        // __HAL_TIM_SetCompare(
        //     htim, TIMER_BRAKE_STEP_CONTROL_CHANNEL, counter + TIM_MS_TO_TICKS(htim, brake_single_step_period));
        //}
    }
}