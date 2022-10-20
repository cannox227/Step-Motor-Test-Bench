/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "brake.h"
#include "pwm.h"
#include "slave_cmd_utils.h"
#include "timer_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ACQUISITION_TIME_DIGITS  2
#define CMD_PAYLOAD_LENGHT       100
#define MAX_CMD_SEND_ATTEMPTS    100
#define MAX_ACK_DELAY_ALLOWED_MS 20
#define CMD_CLOSING_CHAR         '\n'
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
    ASYNC_SENSOR_READING,
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

typedef struct {
    bool is_powered;
} stepper_handler;

stepper_handler stepper;
uint8_t sendings_attempts_done = 0;
uint32_t current_tick          = 0;
uint32_t deadline              = 0;
uint8_t gui_acquisition_time   = 0;
char gui_acquisition_time_str[ACQUISITION_TIME_DIGITS];
float brake_single_step_period = 0.0;
bool is_brake_at_max_value     = false;
bool do_async_sensor_reading   = false;
// ADC variables
volatile float torque_gpio;
volatile float board_voltage;
volatile float stepper_current;
char adc_out[100];

//bool is_sampling = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void set_and_go_to_next_state(master_state next_state);
bool parse_gui_cmd(char *cmd_rx);
void reset_cmd(uart_rx_handler *uart_handler);
void update_sensor_values();
void update_sensor_values_and_print();
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

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_ADC3_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_USART3_UART_Init();
    MX_USART6_UART_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();
    /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ENABLE_STEPPER_SUPPLY_GPIO_Port, ENABLE_STEPPER_SUPPLY_Pin, GPIO_PIN_SET);
    // INIT PHASE
    msm_handler.current_state    = INIT;
    msm_handler.next_state       = WAITING_FOR_CMD;
    msm_handler.go_to_next_state = true;
    reset_cmd(&gui_cmd_handler);
    reset_cmd(&slave_cmd_handler);
    stepper.is_powered = false;

    uint32_t sensor_time = 5000;

    //HAL_GPIO_WritePin(ENABLE_STEPPER_SUPPLY_GPIO_Port, ENABLE_STEPPER_SUPPLY_Pin, GPIO_PIN_RESET);
    memset(adc_out, 0x00, sizeof(adc_out));

    brake_init(&magnetic_brake);
    brake_set_pwm_duty_cycle(&magnetic_brake, 0.0);
    brake_on(&magnetic_brake);

    ADC_start_DMA_readings();

    for (int i = 0; i < 10; i++) {
        measured_zero_current_value = ADC_get_current_to_gpio_level();
        measured_zero_torque_value  = ADC_get_torque_to_gpio_level();
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        // while (1) {
        // while (1) {
        //     HAL_UART_Receive_IT(&UART_USB, (uint8_t *)&gui_cmd_handler.last_char_received, 1);
        //     if (gui_cmd_handler.is_word_complete) {
        //         parse_gui_cmd(gui_cmd_handler.cmd_received);
        //         reset_cmd(&gui_cmd_handler);
        //     }
        //     if (is_sampling) {
        //         brake_single_step_period = gui_acquisition_time / TOTAL_PWM_BRAKE_STEPS;
        //         brake_set_pwm_duty_cycle(&magnetic_brake, 0.0);
        //         brake_set_and_start_step_timer((uint32_t)brake_single_step_period * 1000);
        //         is_sampling = false;
        //     }
        //     if (magnetic_brake.is_top_limit_reached) {
        //         brake_stop_step_timer();
        //         magnetic_brake.is_top_limit_reached = false;
        //     }
        // }
        //     // update_sensor_values_and_print();
        //     // HAL_Delay(100);
        // }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        // while (1) {
        //     HAL_UART_Receive_IT(&UART_USB, (uint8_t *)&gui_cmd_handler.last_char_received, 1);
        //     if (gui_cmd_handler.is_word_complete) {
        //         parse_gui_cmd(gui_cmd_handler.cmd_received);
        //         reset_cmd(&gui_cmd_handler);
        //     }
        //     if (is_sampling) {
        //         brake_single_step_period = gui_acquisition_time / TOTAL_PWM_BRAKE_STEPS;
        //         brake_set_pwm_duty_cycle(&magnetic_brake, 0.0);
        //         brake_set_and_start_step_timer((uint32_t)brake_single_step_period * 1000);
        //         is_sampling = false;
        //     }
        //     if (magnetic_brake.is_top_limit_reached) {
        //         brake_stop_step_timer();
        //         magnetic_brake.is_top_limit_reached = false;
        //     }
        // }
        //     // update_sensor_values_and_print();
        //     // HAL_Delay(100);
        // }
        /* USER CODE END WHILE */
        if (msm_handler.go_to_next_state) {
            msm_handler.current_state = msm_handler.next_state;
        }
        switch (msm_handler.current_state) {
            case INIT:
                HAL_NVIC_SystemReset();
                break;
            case WAITING_FOR_CMD:
                HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_SET);
                // Continuous receiving
                HAL_UART_Receive_IT(&UART_USB, (uint8_t *)&gui_cmd_handler.last_char_received, sizeof(char));
                if (gui_cmd_handler.is_word_complete) {
                    set_and_go_to_next_state(CMD_RECEIVED);
                }
                break;
            case CMD_RECEIVED:
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_SET);
                if (parse_gui_cmd(gui_cmd_handler.cmd_received)) {
                    HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                    if (strstr(gui_cmd_handler.cmd_received, "start_motor+")) {
                        // IDEALLY THE MOTOR RUNS FOREVER
                        set_and_go_to_next_state(SEND_START_MOTOR_CMD);
                    } else if (strcmp(gui_cmd_handler.cmd_received, "get_acquired_data_chunks\r") == 0) {
                        set_and_go_to_next_state(ASYNC_SENSOR_READING);
                    }
                } else {
                    set_and_go_to_next_state(WAITING_FOR_CMD);
                }
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_RESET);
                reset_cmd(&gui_cmd_handler);
                break;
            case ASYNC_SENSOR_READING:
                reset_cmd(&gui_cmd_handler);
                // Continuous receiving and sensor reading
                for (uint8_t i = 0; i < 7; i++) {
                    update_sensor_values_and_print();
                }
                set_and_go_to_next_state(WAITING_FOR_CMD);
                break;
            case SEND_START_MOTOR_CMD:
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_SET);
                if (sendings_attempts_done < MAX_CMD_SEND_ATTEMPTS) {
                    HAL_UART_Transmit(&UART_SLAVE, (uint8_t *)"fw+999999\r\n", sizeof("fw+999999\r\n"), 100);
                    HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_RESET);
                    reset_cmd(&slave_cmd_handler);
                    sendings_attempts_done++;
                    set_and_go_to_next_state(GET_MOTOR_ACK_ON_START);
                } else {
                    set_and_go_to_next_state(WAITING_FOR_CMD);
                    sendings_attempts_done = 0;
                }
                break;
            case GET_MOTOR_ACK_ON_START:
                current_tick = HAL_GetTick();
                deadline     = current_tick + MAX_ACK_DELAY_ALLOWED_MS;
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_SET);
                while (current_tick < deadline) {
                    // set zerto torque value
                    //measured_zero_torque_value = ADC_get_torque_to_gpio_level()*;

                    HAL_UART_Receive_IT(&UART_SLAVE, (uint8_t *)&slave_cmd_handler.last_char_received, sizeof(char));
                    if (slave_cmd_handler.is_word_complete &&
                        strcmp(slave_cmd_handler.cmd_received, "start_ack\r\n") == 0) {
                        reset_cmd(&slave_cmd_handler);
                        reset_cmd(&gui_cmd_handler);
                        set_and_go_to_next_state(MOTOR_IS_RUNNING);
                        HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                        current_tick = deadline;
                    } else {
                        current_tick = HAL_GetTick();
                    }
                }
                // If timeout has elapsed
                if (msm_handler.next_state == GET_MOTOR_ACK_ON_START) {
                    set_and_go_to_next_state(SEND_START_MOTOR_CMD);
                }
                break;
            case MOTOR_IS_RUNNING:
                // do stuffs
                set_and_go_to_next_state(SENSORS_READING);
                current_tick = HAL_GetTick();
                // update deadline to acquired time from gui (s) * 1000 in order to obtain ms
                deadline                 = current_tick + gui_acquisition_time * 1000;  // sensor_time;
                brake_single_step_period = ((float)gui_acquisition_time) / TOTAL_PWM_BRAKE_STEPS;
                brake_set_pwm_duty_cycle(&magnetic_brake, 0.0);
                brake_set_and_start_step_timer((uint32_t)(brake_single_step_period * 1000));
                break;

            case SENSORS_READING:
                while (!magnetic_brake.is_top_limit_reached) {
                    update_sensor_values_and_print();
                }
                brake_stop_step_timer();
                brake_set_pwm_duty_cycle(&magnetic_brake, 0.0);
                magnetic_brake.is_top_limit_reached = false;

                // while (current_tick < deadline) {
                //     update_sensor_values_and_print();
                //     current_tick = HAL_GetTick();
                // }
                // do stuffs
                set_and_go_to_next_state(SEND_STOP_MOTOR_CMD);
                break;
            case SEND_STOP_MOTOR_CMD:
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_SET);
                if (sendings_attempts_done < MAX_CMD_SEND_ATTEMPTS) {
                    HAL_UART_Transmit(&UART_SLAVE, (uint8_t *)MOTOR_STOP_FOREVER, sizeof(MOTOR_STOP_FOREVER), 100);
                    HAL_GPIO_WritePin(DEBUG_SIGNAL_1_GPIO_Port, DEBUG_SIGNAL_1_Pin, GPIO_PIN_RESET);
                    reset_cmd(&slave_cmd_handler);
                    sendings_attempts_done++;
                    set_and_go_to_next_state(GET_MOTOR_ACK_ON_STOP);
                } else {
                    set_and_go_to_next_state(WAITING_FOR_CMD);
                }
                break;
            case GET_MOTOR_ACK_ON_STOP:

                current_tick = HAL_GetTick();
                deadline     = current_tick + MAX_ACK_DELAY_ALLOWED_MS;
                HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_SET);
                while (current_tick < deadline) {
                    HAL_UART_Receive_IT(&UART_SLAVE, (uint8_t *)&slave_cmd_handler.last_char_received, sizeof(char));
                    if (slave_cmd_handler.is_word_complete &&
                        strcmp(slave_cmd_handler.cmd_received, "stop_ack\r\n") == 0) {
                        reset_cmd(&slave_cmd_handler);
                        reset_cmd(&gui_cmd_handler);
                        set_and_go_to_next_state(MOTOR_IS_STOPPED);
                        HAL_GPIO_WritePin(DEBUG_SIGNAL_2_GPIO_Port, DEBUG_SIGNAL_2_Pin, GPIO_PIN_RESET);
                        current_tick = deadline;
                    } else {
                        current_tick = HAL_GetTick();
                    }
                }
                // If timeout has elapsed
                if (msm_handler.next_state == GET_MOTOR_ACK_ON_STOP) {
                    set_and_go_to_next_state(SEND_STOP_MOTOR_CMD);
                }
                break;
            case MOTOR_IS_STOPPED:
                //turn off transistor
                HAL_Delay(2000);
                set_and_go_to_next_state(WAITING_FOR_CMD);
                break;
        }
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

    /** Configure LSE Drive Capability
  */
    HAL_PWR_EnableBkUpAccess();

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 4;
    RCC_OscInitStruct.PLL.PLLN       = 216;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
  */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART_USB.Instance) {
        gui_cmd_handler.cmd_received[gui_cmd_handler.char_index] = gui_cmd_handler.last_char_received;
        gui_cmd_handler.char_index++;
        if (gui_cmd_handler.last_char_received == '\r') {
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

void set_and_go_to_next_state(master_state next_state) {
    msm_handler.next_state       = next_state;
    msm_handler.go_to_next_state = true;
}

void reset_cmd(uart_rx_handler *uart_handler) {
    uart_handler->char_index = 0;
    memset(uart_handler->cmd_received, 0x0, sizeof(uart_handler->cmd_received));
    uart_handler->is_word_complete = false;
}
void send_brake_value_to_gui() {
    char gui_output[15];
    uint8_t len = 0;
    memset(gui_output, 0x00, sizeof(gui_output));
    len = sprintf(gui_output, "PWM+%.2f\r\n", magnetic_brake.pwm_duty_cycle);
    HAL_UART_Transmit(&UART_USB, (uint8_t *)gui_output, len, 100);
}
bool parse_gui_cmd(char *cmd_rx) {
    bool cmd_is_valid = true;
    if (strcmp(cmd_rx, "brake+\r") == 0) {
        if (!magnetic_brake.is_braking) {
            brake_on(&magnetic_brake);
        }
        brake_increase_duty(&magnetic_brake, BRAKE_STEP);
        send_brake_value_to_gui();
    } else if (strcmp(cmd_rx, "brake-\r") == 0) {
        if (!magnetic_brake.is_braking) {
            brake_on(&magnetic_brake);
        }
        brake_decrease_duty(&magnetic_brake, BRAKE_STEP);
        send_brake_value_to_gui();
    } else if (strcmp(cmd_rx, "brake_off\r") == 0) {
        brake_set_pwm_duty_cycle(&magnetic_brake, MIN_ALLOWED_PWM_DUTY);
        brake_off(&magnetic_brake);
        send_brake_value_to_gui();
    } else if (strcmp(cmd_rx, "brake_on_max\r") == 0) {
        brake_set_pwm_duty_cycle(&magnetic_brake, MAX_ALLOWED_PWM_DUTY);
        brake_on(&magnetic_brake);
        send_brake_value_to_gui();
    }
    // else if (strcmp(cmd_rx, "start_motor\r") == 0) {
    //     uint8_t x = 0;
    else if (strstr(cmd_rx, "start_motor+")) {
        // check whether the substring start_motor has been received
        strncpy(gui_acquisition_time_str, &cmd_rx[strlen("start_motor+")], ACQUISITION_TIME_DIGITS);
        gui_acquisition_time = atoi(gui_acquisition_time_str);
        uint8_t x            = 0;
        //is_sampling          = true;
    } else if (strcmp(cmd_rx, "get_acquired_data_chunks\r") == 0) {
        do_async_sensor_reading = true;
    }
    // else if (strcmp(cmd_rx, "stop_async_reading\r") == 0) {
    //     do_async_sensor_reading = false;
    // }
    else {
        cmd_is_valid = false;
    }
    return cmd_is_valid;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if (pin == USER_Btn_Pin) {
        (stepper.is_powered)
            ? HAL_GPIO_WritePin(ENABLE_STEPPER_SUPPLY_GPIO_Port, ENABLE_STEPPER_SUPPLY_Pin, GPIO_PIN_RESET)
            : HAL_GPIO_WritePin(ENABLE_STEPPER_SUPPLY_GPIO_Port, ENABLE_STEPPER_SUPPLY_Pin, GPIO_PIN_SET);
        stepper.is_powered = !stepper.is_powered;
    }
}

void update_sensor_values() {
    torque_gpio     = ADC_get_voltage_to_gpio_level();
    board_voltage   = ADC_get_voltage_converted();
    stepper_current = ADC_get_current_converted();
}

void update_sensor_values_and_print() {
    torque_gpio     = ADC_get_torque_converted();   // ADC_get_torque_to_gpio_level();
    board_voltage   = ADC_get_voltage_converted();  // ADC_get_voltage_to_gpio_level();
    stepper_current = ADC_get_current_converted();  // ADC_get_current_to_gpio_level();
#define MEASUREMENT_PRINT 1
#ifdef DEBUG_PRINT
    sprintf(
        adc_out, "Current: %.4f A | STB supply: %.4f V | Trq: %.4f \r\n", stepper_current, board_voltage, torque_gpio);
#endif
#ifdef MEASUREMENT_PRINT
    sprintf(
        adc_out,
        "[MASTER],%lu,%.4f, %.4f,%.4f, %.2f\r\n",
        HAL_GetTick() / 10,
        torque_gpio,
        board_voltage,
        stepper_current,
        magnetic_brake.pwm_duty_cycle * 100);
#endif
    HAL_UART_Transmit(&UART_USB, (uint8_t *)adc_out, sizeof(adc_out), 100);
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
