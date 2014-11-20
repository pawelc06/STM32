################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/TFT_TEXT/subdir.mk
-include src/TFT_FONT/subdir.mk
-include src/subdir.mk
-include STM32L1xx_StdPeriph_Driver/subdir.mk
-include FatFs_0.10/src/subdir.mk
-include FatFs_0.10/subdir.mk
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
all: ILI_TEST1.elf

# Tool invocations
ILI_TEST1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -T "D:/git/STM32/ILI_TEST1/ILI_TEST1/stm32_flash.ld" -mthumb -mcpu=cortex-m3 -Wl,--gc-sections -o "ILI_TEST1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ILI_TEST1.elf
	-@echo ' '

post-build:
	-@echo 'Hex file build'
	-arm-none-eabi-objcopy -S -O ihex ILI_TEST1.elf ILI_TEST1.hex; arm-none-eabi-size ILI_TEST1.hex
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
