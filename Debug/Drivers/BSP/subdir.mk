################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/powerstep01.c \
../Drivers/BSP/x_nucleo_ihm03a1_stm32f4xx.c \
../Drivers/BSP/x_nucleo_ihmxx.c 

OBJS += \
./Drivers/BSP/powerstep01.o \
./Drivers/BSP/x_nucleo_ihm03a1_stm32f4xx.o \
./Drivers/BSP/x_nucleo_ihmxx.o 

C_DEPS += \
./Drivers/BSP/powerstep01.d \
./Drivers/BSP/x_nucleo_ihm03a1_stm32f4xx.d \
./Drivers/BSP/x_nucleo_ihmxx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o: ../Drivers/BSP/%.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -DTX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/threadx/utility/low_power/ -I../AZURE_RTOS/App -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/ -I../Drivers/BSP -I../Tasks/Powerstep -I../Tasks/Serial -I../Tasks/Torque -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/powerstep01.d ./Drivers/BSP/powerstep01.o ./Drivers/BSP/x_nucleo_ihm03a1_stm32f4xx.d ./Drivers/BSP/x_nucleo_ihm03a1_stm32f4xx.o ./Drivers/BSP/x_nucleo_ihmxx.d ./Drivers/BSP/x_nucleo_ihmxx.o

.PHONY: clean-Drivers-2f-BSP

