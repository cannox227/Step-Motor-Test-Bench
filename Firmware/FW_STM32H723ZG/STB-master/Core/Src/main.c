/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "adc.h"
#include "eth.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "brake.h"
#include "pwm.h"
#include "slave_cmd_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CMD_PAYLOAD_LENGHT       100
#define MAX_CMD_SEND_ATTEMPTS    100
#define MAX_ACK_DELAY_ALLOWED_MS 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

typedef struct {
    bool is_word_complete;
    char cmd_received[CMD_PAYLOAD_LENGHT];
    char last_char_received;
    uint8_t char_index;
} uart_rx_handler;

uart_rx_handler gui_cmd_handler;
uart_rx_handler slave_cmd_handler;

typedef enum {
    INIT = 0U,
    WAITING_FOR_CMD,
    CMD_RECEIVED,
    SEND_START_MOTOR_CMD,
    GET_MOTOR_ACK_ON_START,
    MOTOR_IS_RUNNING,
    SENSORS_READING,
    SEND_STOP_MOTOR_CMD,
    GET_MOTOR_ACK_ON_STOP,
    MOTOR_IS_STOPPED
} master_state;

typedef struct {
    master_state current_state;
    master_state next_state;
    bool go_to_next_state;
} master_state_machine;

master_state_machine msm_handler;

uint8_t sendings_attempts_done = 0;
uint32_t current_tick          = 0;
uint32_t deadline              = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
bool parse_gui_cmd(char *cmd_rx);
void reset_cmd(uart_rx_handler *uart_handler);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */

    // INIT PHASE
    msm_handler.current_state    = INIT;
    msm_handler.next_state       = WAITING_FOR_CMD;
    msm_handler.go_to_next_state = true;
    reset_cmd(&gui_cmd_handler);
    reset_cmd(&slave_cmd_handler);

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ETH_Init();
    MX_USART3_UART_Init();
    MX_USB_OTG_HS_USB_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_ADC3_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */

    brake_init(&magnetic_brake);
    brake_set_pwm_duty_cycle(&magnetic_brake, 0.5);

    //brake_on(&magnetic_brake);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        if (msm_handler.go_to_next_state) {
            msm_handler.current_state = msm_handler.next_state;
        }
        switch (msm_handler.current_state) {
            case INIT:
                HAL_NVIC_SystemReset();
                break;
            case WAITING_FOR_CMD:
                // Continuous receiving
                HAL_UART_Receive_IT(&UART_USB, (uint8_t *)&gui_cmd_handler.last_char_received, sizeof(char));
                if (gui_cmd_handler.is_word_complete) {
                    msm_handler.next_state       = CMD_RECEIVED;
                    msm_handler.go_to_next_state = true;
                }
                break;
            case CMD_RECEIVED:
                if (parse_gui_cmd(gui_cmd_handler.cmd_received) &&
                    strcmp(gui_cmd_handler.cmd_received, "start_motor\r\n") == 0) {
                    // IDEALLY THE MOTOR RUNS FOREVER
                    msm_handler.next_state       = SEND_START_MOTOR_CMD;
                    msm_handler.go_to_next_state = true;
                } else {
                    msm_handler.next_state       = WAITING_FOR_CMD;
                    msm_handler.go_to_next_state = true;
                }
                reset_cmd(&gui_cmd_handler);
                break;
            case SEND_START_MOTOR_CMD:
                if (sendings_attempts_done < MAX_CMD_SEND_ATTEMPTS) {
                    HAL_UART_Transmit(&UART_SLAVE, (uint8_t *)"fw+999999\r\n", sizeof("fw+999999\r\n"), 100);
                    reset_cmd(&slave_cmd_handler);
                    sendings_attempts_done++;
                    msm_handler.next_state       = GET_MOTOR_ACK_ON_START;
                    msm_handler.go_to_next_state = true;
                } else {
                    msm_handler.next_state       = WAITING_FOR_CMD;
                    msm_handler.go_to_next_state = true;
                    sendings_attempts_done       = 0;
                }
                break;
            case GET_MOTOR_ACK_ON_START:
                current_tick = HAL_GetTick();
                deadline     = current_tick + 10 * MAX_ACK_DELAY_ALLOWED_MS;
                while (current_tick < deadline) {
                    HAL_UART_Receive_IT(&UART_SLAVE, (uint8_t *)&slave_cmd_handler.last_char_received, sizeof(char));
                    if (slave_cmd_handler.is_word_complete &&
                        strcmp(slave_cmd_handler.cmd_received, "start_ack\r\n") == 0) {
                        reset_cmd(&slave_cmd_handler);
                        reset_cmd(&gui_cmd_handler);
                        msm_handler.next_state       = MOTOR_IS_RUNNING;
                        msm_handler.go_to_next_state = true;
                        current_tick                 = deadline;
                    } else {
                        current_tick = HAL_GetTick();
                    }
                }
                // If timeout has elapsed
                if (msm_handler.next_state == GET_MOTOR_ACK_ON_START) {
                    msm_handler.next_state       = SEND_START_MOTOR_CMD;
                    msm_handler.go_to_next_state = true;
                }
                break;
            case MOTOR_IS_RUNNING:
                // do stuffs
                msm_handler.next_state       = SENSORS_READING;
                msm_handler.go_to_next_state = true;
                break;

            case SENSORS_READING:
                // do stuffs
                //HAL_Delay(5000);
                msm_handler.next_state       = SEND_STOP_MOTOR_CMD;
                msm_handler.go_to_next_state = true;
                break;
            case SEND_STOP_MOTOR_CMD:
                if (sendings_attempts_done < MAX_CMD_SEND_ATTEMPTS) {
                    HAL_UART_Transmit(&UART_SLAVE, (uint8_t *)MOTOR_STOP_FOREVER, sizeof(MOTOR_STOP_FOREVER), 100);
                    reset_cmd(&slave_cmd_handler);
                    sendings_attempts_done++;
                    msm_handler.next_state       = GET_MOTOR_ACK_ON_STOP;
                    msm_handler.go_to_next_state = true;
                } else {
                    msm_handler.next_state       = WAITING_FOR_CMD;
                    msm_handler.go_to_next_state = true;
                }
                break;
            case GET_MOTOR_ACK_ON_STOP:
                current_tick = HAL_GetTick();
                deadline     = current_tick + 10 * MAX_ACK_DELAY_ALLOWED_MS;
                while (current_tick < deadline) {
                    HAL_UART_Receive_IT(&UART_SLAVE, (uint8_t *)&slave_cmd_handler.last_char_received, sizeof(char));
                    if (slave_cmd_handler.is_word_complete &&
                        strcmp(slave_cmd_handler.cmd_received, "stop_ack\r\n") == 0) {
                        reset_cmd(&slave_cmd_handler);
                        reset_cmd(&gui_cmd_handler);
                        msm_handler.next_state       = MOTOR_IS_STOPPED;
                        msm_handler.go_to_next_state = true;
                        current_tick                 = deadline;
                    } else {
                        current_tick = HAL_GetTick();
                    }
                }
                // If timeout has elapsed
                if (msm_handler.next_state == GET_MOTOR_ACK_ON_STOP) {
                    msm_handler.next_state       = SEND_STOP_MOTOR_CMD;
                    msm_handler.go_to_next_state = true;
                }
                break;
            case MOTOR_IS_STOPPED:
                //turn off transistor
                HAL_Delay(2000);
                msm_handler.next_state       = WAITING_FOR_CMD;
                msm_handler.go_to_next_state = true;
                break;
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
   */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
   */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
    }
    /** Macro to configure the PLL clock source
   */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 4;
    RCC_OscInitStruct.PLL.PLLN       = 275;
    RCC_OscInitStruct.PLL.PLLP       = 1;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
   */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Initializes the peripherals clock
   */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M           = 1;
    PeriphClkInitStruct.PLL2.PLL2N           = 24;
    PeriphClkInitStruct.PLL2.PLL2P           = 2;
    PeriphClkInitStruct.PLL2.PLL2Q           = 2;
    PeriphClkInitStruct.PLL2.PLL2R           = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART_USB.Instance) {
        gui_cmd_handler.cmd_received[gui_cmd_handler.char_index] = gui_cmd_handler.last_char_received;
        gui_cmd_handler.char_index++;
        if (gui_cmd_handler.last_char_received == '\n') {
            gui_cmd_handler.is_word_complete = true;
        }
    } else if (huart->Instance == UART_SLAVE.Instance) {
        slave_cmd_handler.cmd_received[slave_cmd_handler.char_index] = slave_cmd_handler.last_char_received;
        slave_cmd_handler.char_index++;
        if (slave_cmd_handler.last_char_received == '\n') {
            slave_cmd_handler.is_word_complete = true;
        }
    }
}

void reset_cmd(uart_rx_handler *uart_handler) {
    uart_handler->char_index = 0;
    memset(uart_handler->cmd_received, 0x0, sizeof(uart_handler->cmd_received));
    uart_handler->is_word_complete = false;
}

bool parse_gui_cmd(char *cmd_rx) {
    bool cmd_is_valid = true;
    char gui_output[12];
    if (strcmp(cmd_rx, "brake+\r\n") == 0) {
        if (!magnetic_brake.is_braking) {
            brake_on(&magnetic_brake);
        }
        brake_increase_duty(&magnetic_brake, BRAKE_STEP);
        memset(gui_output, 0x00, sizeof(gui_output));
        sprintf(gui_output, "PWM+%.2f\r\n", magnetic_brake.pwm_duty_cycle);
        HAL_UART_Transmit(&UART_USB, (uint8_t *)gui_output, sizeof(gui_output), 100);
    } else if (strcmp(cmd_rx, "brake-\r\n") == 0) {
        if (!magnetic_brake.is_braking) {
            brake_on(&magnetic_brake);
        }
        brake_decrease_duty(&magnetic_brake, BRAKE_STEP);
        memset(gui_output, 0x00, sizeof(gui_output));
        sprintf(gui_output, "PWM+%.2f\r\n", magnetic_brake.pwm_duty_cycle);
        HAL_UART_Transmit(&UART_USB, (uint8_t *)gui_output, sizeof(gui_output), 100);
    } else if (strcmp(cmd_rx, "brake_off\r\n") == 0) {
        brake_set_pwm_duty_cycle(&magnetic_brake, MIN_ALLOWED_PWM_DUTY);
        brake_off(&magnetic_brake);
    } else if (strcmp(cmd_rx, "brake_on_max\r\n") == 0) {
        brake_set_pwm_duty_cycle(&magnetic_brake, MAX_ALLOWED_PWM_DUTY);
        brake_on(&magnetic_brake);
    } else if (strcmp(cmd_rx, "start_motor\r\n") == 0) {
    } else {
        cmd_is_valid = false;
    }
    return cmd_is_valid;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
