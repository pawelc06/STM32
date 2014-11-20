################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include STM32F0xx_StdPeriph_Driver/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ARM_TemplateL100

# Tool invocations
ARM_TemplateL100: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -T "D:/arm_projects/eclipse_workspace/ARM_TemplateL100/stm32_flash.ld" -mthumb -mcpu=cortex-m3 -Wl,--gc-sections -o "ARM_TemplateL100" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ILI_TEST1
	-@echo ' '

post-build:
	-@echo 'Hex file build'
	-arm-none-eabi-objcopy -S -O ihex ARM_TemplateL100 ARM_TemplateL100.hex; arm-none-eabi-size ILI_TEST1
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
