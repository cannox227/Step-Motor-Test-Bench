/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc2;

extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */

#define ADC1_CONVERSIONS    16
#define ADC2_CONVERSIONS    16
#define ADC3_CONVERSIONS    16
#define GPIO_MAX_VOLTAGE    3.3f
#define ADC_BITS_RESOLUTION 12
#define ADC_MAX_RESOLUTION  4096.0f

/** 
  * @brief Max GPIO voltage measured when the STB board is powered with 24V
  */
#define GPIO_MAX_VOLTAGE_MEASURED_FROM_VOLTAGE_SENSOR 3.126f  //3.16f

/** 
  * @brief Voltage value measured when the stepper it's not enabled
  */
#define CURRENT_SENSOR_OFFSET 1.617f  //1.6290f

#define CURRENT_DIVIDER_R_TOP    330
#define CURRENT_DIVIDER_R_BOTTOM 620
#define CURRENT_DIVIDER_FACTOR \
    (float)(CURRENT_DIVIDER_R_TOP + CURRENT_DIVIDER_R_BOTTOM) / (float)(CURRENT_DIVIDER_R_BOTTOM)

/** 
  * @brief Theoretical sensitivity expressed in mV/A
  * Sensitivity: If in the transducer flows 1A of current then the output
  * voltage vould be Vout=Vref+(1A*Sensitivity)
  */
#define LEM_CAS_6_NP_THEORETICAL_SENSITIVITY 104.2f

// Torque sensor params

#define TORQUE_DIVIDER_R_TOP    330
#define TORQUE_DIVIDER_R_BOTTOM 620
#define TORQUE_DIVIDER_FACTOR   (float)(TORQUE_DIVIDER_R_TOP + TORQUE_DIVIDER_R_BOTTOM) / (float)(TORQUE_DIVIDER_R_BOTTOM)
#define SLOPE_TORQUE_SENSOR     781.43  //[mV/Nm]

// Hardcoded voltage value corresponding to zero torque [Nm]
#define TORQUE_ZERO_IN_VOLTS 2.46f
/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */
extern float measured_zero_torque_value;
extern float measured_zero_current_value;

void ADC_Calibration();
void ADC_start_DMA_readings();
uint16_t ADC_get_current_raw();
float ADC_get_current_to_gpio_level();
float ADC_get_current_converted();
uint16_t ADC_get_torque_raw();
float ADC_get_torque_to_gpio_level();
float ADC_get_torque_converted();
uint16_t ADC_get_voltage_raw();
float ADC_get_voltage_to_gpio_level();
float ADC_get_voltage_converted();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

