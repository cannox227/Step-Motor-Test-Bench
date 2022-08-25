/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#define ADC1_CONVERSIONS        16
#define GPIO_MAX_VOLTAGE        3.3f
#define ADC_MAX_RESOLUTION      4096.0f
#define TORQUE_DIVIDER_R_TOP    330
#define TORQUE_DIVIDER_R_BOTTOM 620
#define TORQUE_DIVIDER_FACTOR   (float)(TORQUE_DIVIDER_R_TOP + TORQUE_DIVIDER_R_BOTTOM) / (float)(TORQUE_DIVIDER_R_BOTTOM)
uint16_t torque_buff[ADC1_CONVERSIONS] = {};

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void) {
    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
    hadc1.Instance                   = ADC1;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode          = DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
    sConfig.Channel      = ADC_CHANNEL_0;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
        GPIO_InitStruct.Pin  = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance                 = DMA2_Stream0;
        hdma_adc1.Init.Channel             = DMA_CHANNEL_0;
        hdma_adc1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode                = DMA_CIRCULAR;
        hdma_adc1.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_adc1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {
    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
void ADC_start_DMA_readings() {
    if (HAL_ADC_Start_DMA(&ADC_TORQUE, (uint32_t *)torque_buff, ADC1_CONVERSIONS) != HAL_OK) {
        Error_Handler();
    }
    HAL_TIM_PWM_Start_IT(&TIMER_ADC_TRIGGER_CONVERSIONS, TIMER_ADC_CHANNEL_TORQUE_TRIGGER);
}

/**
 * @brief Return raw ADC value of torque sensor
 * 
 * @return uint16_t 
 */
uint16_t ADC_get_torque_raw() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < ADC1_CONVERSIONS; i++) {
        result += torque_buff[i];
    }
    return (uint16_t)(result / ADC1_CONVERSIONS);
}

/**
 * @brief Return ADC torque sensor converted from raw to GPIO level (max 3.3V)
 * 
 * @return float 
 */
float ADC_get_torque_to_gpio_level() {
    uint16_t raw_val = ADC_get_torque_raw();
    //raw val : 4095 = v : 3.3
    return (float)(raw_val * GPIO_MAX_VOLTAGE / ADC_MAX_RESOLUTION) * TORQUE_DIVIDER_FACTOR;
}
/* USER CODE END 1 */
