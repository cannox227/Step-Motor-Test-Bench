#include "pwm.h"
#include "timer_utils.h"

void pwm_set_period(TIM_HandleTypeDef *htim, float period_ms)
{
    __HAL_TIM_SetAutoreload(htim, TIM_MS_TO_TICKS(htim, period_ms)); // set the period
}

void pwm_set_duty_cicle(TIM_HandleTypeDef *htim, uint32_t channel, float duty_cicle)
{
    if (duty_cicle < 0 || duty_cicle > 1)
        return;

    __HAL_TIM_SetCompare(htim, channel, __HAL_TIM_GetAutoreload(htim) * duty_cicle);
}

void pwm_start_channel(TIM_HandleTypeDef *htim, uint32_t channel)
{
    HAL_TIM_PWM_Start_IT(htim, channel);
}

void pwm_stop_channel(TIM_HandleTypeDef *htim, uint32_t channel)
{
    HAL_TIM_PWM_Stop_IT(htim, channel);
}