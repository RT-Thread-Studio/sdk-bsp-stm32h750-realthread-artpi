################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rt-thread/src/subdir.mk
-include rt-thread/libcpu/arm/cortex-m7/subdir.mk
-include rt-thread/libcpu/arm/common/subdir.mk
-include rt-thread/components/libc/compilers/newlib/subdir.mk
-include rt-thread/components/finsh/subdir.mk
-include rt-thread/components/drivers/src/subdir.mk
-include rt-thread/components/drivers/spi/subdir.mk
-include rt-thread/components/drivers/serial/subdir.mk
-include rt-thread/components/drivers/misc/subdir.mk
-include rt-thread/components/dfs/src/subdir.mk
-include rt-thread/components/dfs/filesystems/devfs/subdir.mk
-include libraries/drivers/subdir.mk
-include libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Src/subdir.mk
-include libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/subdir.mk
-include libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/subdir.mk
-include board/port/subdir.mk
-include board/CubeMX_Config/Core/Src/subdir.mk
-include board/subdir.mk
-include applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rtthread.bin \

SECONDARY_SIZE += \
rtthread.siz \


# 所有目标
all: rtthread.elf secondary-outputs

# 工具调用
rtthread.elf: $(OBJS) $(USER_OBJS)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -T "D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\linker_scripts\STM32H750XBHx\link.lds" -Xlinker --gc-sections -Wl,-Map,"rtthread.map" -Xlinker --cref -o "rtthread.elf" $(OBJS) $(USER_OBJS) $(LIBS)

rtthread.bin: rtthread.elf
	arm-none-eabi-objcopy -O binary "rtthread.elf"  "rtthread.bin"

rtthread.siz: rtthread.elf
	arm-none-eabi-size --format=berkeley "rtthread.elf"

# 其他目标
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) rtthread.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
