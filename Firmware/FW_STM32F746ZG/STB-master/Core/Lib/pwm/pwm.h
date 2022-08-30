#ifndef PWM_H
#define PWM_H

#include "main.h"
#include <inttypes.h>

/**
 * @brief     Set the period for the specified timer. This will affect every channel
 *
 * @param     htim TIM Handle
 * @param     period_ms period of the timer in milliseconds
 */
void pwm_set_period(TIM_HandleTypeDef *htim, float period_ms);

/**
 * @brief     Set the duty cicle for the specified channel. Use this function only to generate a plain PWM (needs to call pwm_start_channel)
 *
 * @param     htim TIM Handle
 * @param     channel timer channel
 * @param     duty_cicle duty cicle of the pwm. have to be between 0 and 1
 */
void pwm_set_duty_cicle(TIM_HandleTypeDef *htim, uint32_t channel, float duty_cicle);

/**
 * @brief     Starts the specified channel
 *
 * @param     htim TIM Handle
 * @param     channel timer channel
 */
void pwm_start_channel(TIM_HandleTypeDef *htim, uint32_t channel);

/**
 * @brief     Stops the specified channel
 *
 * @param     htim TIM Handle
 * @param     channel timer channel
 */
void pwm_stop_channel(TIM_HandleTypeDef *htim, uint32_t channel);

#endif