################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32l1xx_mdp.S 

OBJS += \
./startup/startup_stm32l1xx_mdp.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -mthumb -mcpu=cortex-m3 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


