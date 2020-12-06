################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h750xx.s 

OBJS += \
./libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h750xx.o 

S_DEPS += \
./libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h750xx.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/%.o: ../libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/%.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -x assembler-with-cpp -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc" -Xassembler -mimplicit-it=thumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

