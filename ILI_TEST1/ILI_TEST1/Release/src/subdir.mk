################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32l100c_discovery.c \
../src/stm32l1xx_it.c \
../src/system_stm32l1xx.c 

OBJS += \
./src/main.o \
./src/stm32l100c_discovery.o \
./src/stm32l1xx_it.o \
./src/system_stm32l1xx.o 

C_DEPS += \
./src/main.d \
./src/stm32l100c_discovery.d \
./src/stm32l1xx_it.d \
./src/system_stm32l1xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MDP -I"D:\arm_projects\eclipse_workspace\ARM_TemplateL100\inc" -I"D:\arm_projects\STM32L1xx_StdPeriph_Lib_V1.2.0\Libraries\CMSIS\Include" -I"D:\arm_projects\STM32L1xx_StdPeriph_Lib_V1.2.0\Libraries\CMSIS\Device\ST\STM32L1xx\Include" -I"D:\arm_projects\STM32L1xx_StdPeriph_Lib_V1.2.0\Libraries\STM32L1xx_StdPeriph_Driver\inc" -Os -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


