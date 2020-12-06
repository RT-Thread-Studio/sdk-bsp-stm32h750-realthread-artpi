################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/common/backtrace.c \
../rt-thread/libcpu/arm/common/div0.c \
../rt-thread/libcpu/arm/common/showmem.c 

OBJS += \
./rt-thread/libcpu/arm/common/backtrace.o \
./rt-thread/libcpu/arm/common/div0.o \
./rt-thread/libcpu/arm/common/showmem.o 

C_DEPS += \
./rt-thread/libcpu/arm/common/backtrace.d \
./rt-thread/libcpu/arm/common/div0.d \
./rt-thread/libcpu/arm/common/showmem.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/common/%.o: ../rt-thread/libcpu/arm/common/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\applications" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\CubeMX_Config\Core\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\port" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include\config" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\filesystems\devfs" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\spi" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\finsh" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\libc\compilers\newlib" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\common" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\cortex-m7" -include"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

