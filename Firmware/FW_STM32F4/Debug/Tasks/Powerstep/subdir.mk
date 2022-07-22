################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tasks/Powerstep/PowerStep01_Task.c \
../Tasks/Powerstep/Powerstep_Task.c 

OBJS += \
./Tasks/Powerstep/PowerStep01_Task.o \
./Tasks/Powerstep/Powerstep_Task.o 

C_DEPS += \
./Tasks/Powerstep/PowerStep01_Task.d \
./Tasks/Powerstep/Powerstep_Task.d 


# Each subdirectory must supply rules for building sources it contributes
Tasks/Powerstep/%.o: ../Tasks/Powerstep/%.c Tasks/Powerstep/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -DTX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/threadx/utility/low_power/ -I../AZURE_RTOS/App -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/ -I../Drivers/BSP -I../Tasks/Powerstep -I../Tasks/Serial -I../Tasks/Torque -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

