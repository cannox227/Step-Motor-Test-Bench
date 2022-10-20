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
#include "timer_utils.h"

// Note that mV / 1000 = V !

// Measured voltage value corresponding to zero torque [Nm]
float measured_zero_torque_value;
// Measured voltage value corresponding to current [A]
float measured_zero_current_value;

// Arrays where the ADC readings are stored

uint16_t current_buff[ADC1_CONVERSIONS] = {};
uint16_t torque_buff[ADC1_CONVERSIONS]  = {};
uint16_t voltage_buff[ADC1_CONVERSIONS] = {};
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

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
    hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc1.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
    sConfig.Channel      = ADC_CHANNEL_3;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}
/* ADC2 init function */
void MX_ADC2_Init(void) {
    /* USER CODE BEGIN ADC2_Init 0 */

    /* USER CODE END ADC2_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC2_Init 1 */

    /* USER CODE END ADC2_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
    hadc2.Instance                   = ADC2;
    hadc2.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc2.Init.ContinuousConvMode    = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc2.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC2;
    hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion       = 1;
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc2) != HAL_OK) {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
    sConfig.Channel      = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC2_Init 2 */

    /* USER CODE END ADC2_Init 2 */
}
/* ADC3 init function */
void MX_ADC3_Init(void) {
    /* USER CODE BEGIN ADC3_Init 0 */

    /* USER CODE END ADC3_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC3_Init 1 */

    /* USER CODE END ADC3_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
    hadc3.Instance                   = ADC3;
    hadc3.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc3.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc3.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc3.Init.ContinuousConvMode    = DISABLE;
    hadc3.Init.DiscontinuousConvMode = DISABLE;
    hadc3.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
    hadc3.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC3;
    hadc3.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc3.Init.NbrOfConversion       = 1;
    hadc3.Init.DMAContinuousRequests = ENABLE;
    hadc3.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc3) != HAL_OK) {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
    sConfig.Channel      = ADC_CHANNEL_13;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC3_Init 2 */

    /* USER CODE END ADC3_Init 2 */
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
    PA3     ------> ADC1_IN3
    */
        GPIO_InitStruct.Pin  = TORQUE_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(TORQUE_ADC_GPIO_Port, &GPIO_InitStruct);

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
    } else if (adcHandle->Instance == ADC2) {
        /* USER CODE BEGIN ADC2_MspInit 0 */

        /* USER CODE END ADC2_MspInit 0 */
        /* ADC2 clock enable */
        __HAL_RCC_ADC2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC2 GPIO Configuration
    PA5     ------> ADC2_IN5
    */
        GPIO_InitStruct.Pin  = CURRENT_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(CURRENT_ADC_GPIO_Port, &GPIO_InitStruct);

        /* ADC2 DMA Init */
        /* ADC2 Init */
        hdma_adc2.Instance                 = DMA2_Stream2;
        hdma_adc2.Init.Channel             = DMA_CHANNEL_1;
        hdma_adc2.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc2.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc2.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc2.Init.Mode                = DMA_CIRCULAR;
        hdma_adc2.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_adc2.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc2) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc2);

        /* ADC2 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC2_MspInit 1 */

        /* USER CODE END ADC2_MspInit 1 */
    } else if (adcHandle->Instance == ADC3) {
        /* USER CODE BEGIN ADC3_MspInit 0 */

        /* USER CODE END ADC3_MspInit 0 */
        /* ADC3 clock enable */
        __HAL_RCC_ADC3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**ADC3 GPIO Configuration
    PC3     ------> ADC3_IN13
    */
        GPIO_InitStruct.Pin  = STEPPER_VOLTAGE_ADC_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(STEPPER_VOLTAGE_ADC_GPIO_Port, &GPIO_InitStruct);

        /* ADC3 DMA Init */
        /* ADC3 Init */
        hdma_adc3.Instance                 = DMA2_Stream1;
        hdma_adc3.Init.Channel             = DMA_CHANNEL_2;
        hdma_adc3.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_adc3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_adc3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc3.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_adc3.Init.Mode                = DMA_CIRCULAR;
        hdma_adc3.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_adc3.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc3) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc3);

        /* ADC3 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC3_MspInit 1 */

        /* USER CODE END ADC3_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {
    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
        HAL_GPIO_DeInit(TORQUE_ADC_GPIO_Port, TORQUE_ADC_Pin);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC1 interrupt Deinit */
        /* USER CODE BEGIN ADC1:ADC_IRQn disable */
        /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC1:ADC_IRQn disable */

        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    } else if (adcHandle->Instance == ADC2) {
        /* USER CODE BEGIN ADC2_MspDeInit 0 */

        /* USER CODE END ADC2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC2_CLK_DISABLE();

        /**ADC2 GPIO Configuration
    PA5     ------> ADC2_IN5
    */
        HAL_GPIO_DeInit(CURRENT_ADC_GPIO_Port, CURRENT_ADC_Pin);

        /* ADC2 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC2 interrupt Deinit */
        /* USER CODE BEGIN ADC2:ADC_IRQn disable */
        /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC2:ADC_IRQn disable */

        /* USER CODE BEGIN ADC2_MspDeInit 1 */

        /* USER CODE END ADC2_MspDeInit 1 */
    } else if (adcHandle->Instance == ADC3) {
        /* USER CODE BEGIN ADC3_MspDeInit 0 */

        /* USER CODE END ADC3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC3_CLK_DISABLE();

        /**ADC3 GPIO Configuration
    PC3     ------> ADC3_IN13
    */
        HAL_GPIO_DeInit(STEPPER_VOLTAGE_ADC_GPIO_Port, STEPPER_VOLTAGE_ADC_Pin);

        /* ADC3 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC3 interrupt Deinit */
        /* USER CODE BEGIN ADC3:ADC_IRQn disable */
        /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
        /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
        /* USER CODE END ADC3:ADC_IRQn disable */

        /* USER CODE BEGIN ADC3_MspDeInit 1 */

        /* USER CODE END ADC3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
void ADC_Calibration() {
// Instead of H7 boards, F746ZG does not have ADC calibration,
#ifdef H7_BOARD
    if (HAL_ADCEx_Calibration_Start(&ADC_CURRENT) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADCEx_Calibration_Start(&ADC_TORQUE) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADCEx_Calibration_Start(&ADC_VOLTAGE) != HAL_OK) {
        Error_Handler();
    }
#endif
}

void ADC_start_DMA_readings() {
    if (HAL_ADC_Start_DMA(&ADC_CURRENT, (uint32_t *)current_buff, ADC1_CONVERSIONS) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADC_Start_DMA(&ADC_TORQUE, (uint32_t *)torque_buff, ADC2_CONVERSIONS) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ADC_Start_DMA(&ADC_VOLTAGE, (uint32_t *)voltage_buff, ADC3_CONVERSIONS)) {
        Error_Handler();
    }

    // status                            = HAL_ADC_Start_DMA(&ADC_TORQUE, (uint32_t *)torque_buff, ADC2_CONVERSIONS);
    // status                            = HAL_ADC_Start_DMA(&ADC_VOLTAGE, (uint32_t *)voltage_buff, ADC3_CONVERSIONS);

    //PWM start DMA peripheral to memory transfer
    HAL_TIM_PWM_Start_IT(&TIMER_ADC_TRIGGER_CONVERSIONS, TIMER_ADC_CHANNEL_CURRENT_TRIGGER);
    HAL_TIM_PWM_Start_IT(&TIMER_ADC_TRIGGER_CONVERSIONS, TIMER_ADC_CHANNEL_TORQUE_TRIGGER);
    HAL_TIM_PWM_Start_IT(&TIMER_ADC_TRIGGER_CONVERSIONS, TIMER_ADC_CHANNEL_VOLTAGE_TRIGGER);
}

/**
 * @brief Return raw ADC value of current sensor
 * 
 * @return uint16_t 
 */
uint16_t ADC_get_current_raw() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < ADC1_CONVERSIONS; i++) {
        result += current_buff[i];
    }
    return (uint16_t)(result / ADC1_CONVERSIONS);
}

/**
 * @brief Return ADC current sensor converted from raw to GPIO level (max 3.3V)
 * 
 * @return float 
 */
float ADC_get_current_to_gpio_level() {
    uint16_t raw_val = ADC_get_current_raw();
    //raw val : 4095 = v : 3.3
    return (float)(raw_val * GPIO_MAX_VOLTAGE / ADC_MAX_RESOLUTION);
}

/**
 * @brief Return current sensor in A
 * 
 * @return float current (A)
 */
float ADC_get_current_converted() {
#define HARDCODED_CURRENT_OFFSET 1
#ifdef HARDCODED_CURRENT_OFFSET
    float gpio_val = ADC_get_current_to_gpio_level() - CURRENT_SENSOR_OFFSET;
#endif
#ifdef REAL_MEASUREMENT_CURRENT_OFFSET
    float gpio_val = ADC_get_current_to_gpio_level() - measured_zero_current_value;
#endif
    float current_A = (gpio_val / LEM_CAS_6_NP_THEORETICAL_SENSITIVITY) * 1000 *
                      CURRENT_DIVIDER_FACTOR;  // ( V / [mV/A] ) * 1000
    return current_A;
}

/**
 * @brief Return raw ADC value of torque sensor
 * 
 * @return uint16_t 
 */
uint16_t ADC_get_torque_raw() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < ADC2_CONVERSIONS; i++) {
        result += torque_buff[i];
    }
    return (uint16_t)(result / ADC2_CONVERSIONS);
}

/**
 * @brief Return ADC torque sensor converted from raw to GPIO level (max 3.3V)
 * 
 * @return float 
 */
float ADC_get_torque_to_gpio_level() {
    uint16_t raw_val = ADC_get_torque_raw();
    //raw val : 4095 = v : 3.3
    return (float)(raw_val * GPIO_MAX_VOLTAGE / ADC_MAX_RESOLUTION);
}

/**
 * @brief Return raw ADC value of STB supply voltage
 * 
 * @return uint16_t 
 */
uint16_t ADC_get_voltage_raw() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < ADC3_CONVERSIONS; i++) {
        result += voltage_buff[i];
    }
    return (uint16_t)(result / ADC3_CONVERSIONS);
}

/**
 * @brief Return ADC value of STB supply voltage converted from raw to GPIO level (max 3.3V)
 * 
 * @return float 
 */
float ADC_get_voltage_to_gpio_level() {
    uint16_t raw_val = ADC_get_voltage_raw();
    //raw val : 4095 = v : 3.3
    return (float)(raw_val * GPIO_MAX_VOLTAGE / ADC_MAX_RESOLUTION);
}
/**
 * @brief Return value of STB supply voltage converted from GPIO level to max power supply (24V)
 * 
 * @return float 
 */
float ADC_get_voltage_converted() {
    volatile float raw_val = ADC_get_voltage_to_gpio_level();
    // raw_val : 3.3 = x : 24
    return (float)(raw_val * MAX_STEPPER_SUPPLY / GPIO_MAX_VOLTAGE_MEASURED_FROM_VOLTAGE_SENSOR);
}
/**
 * @brief Return value of torque sensor converted in Nm 
 * 
 * @return float 
 */
float ADC_get_torque_converted() {
#define HARDCODED_TORQUE_OFFSET 1
    float raw_val = ADC_get_torque_to_gpio_level();
#ifdef HARDCODED_TORQUE_OFFSET
    return ((raw_val * TORQUE_DIVIDER_FACTOR) - TORQUE_ZERO_IN_VOLTS) /
           (SLOPE_TORQUE_SENSOR / 1000);  // [V]/([mV/Nm]/1000) -> Nm
#endif
#ifdef REAL_MEASUREMENT_TORQUE_OFFSET
    return ((raw_val * TORQUE_DIVIDER_FACTOR) - measured_zero_torque_value) /
           (SLOPE_TORQUE_SENSOR / 1000);  // [V]/([mV/Nm]/1000) -> Nm
#endif
}

// // Called when buffer is completely filled
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
//     if (hadc->Instance == ADC_CURRENT.Instance) {
//         HAL_GPIO_TogglePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin);
//     } else if (hadc->Instance == ADC_TORQUE.Instance) {
//         HAL_GPIO_TogglePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin);
//     } else if (hadc->Instance == ADC_VOLTAGE.Instance) {
//         HAL_GPIO_TogglePin(DEBUG_PIN_3_GPIO_Port, DEBUG_PIN_3_Pin);
//     }
// }

/* USER CODE END 1 */
