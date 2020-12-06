################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/libc/compilers/newlib/libc.c \
../rt-thread/components/libc/compilers/newlib/libc_syms.c \
../rt-thread/components/libc/compilers/newlib/stdio.c \
../rt-thread/components/libc/compilers/newlib/syscalls.c \
../rt-thread/components/libc/compilers/newlib/time.c 

OBJS += \
./rt-thread/components/libc/compilers/newlib/libc.o \
./rt-thread/components/libc/compilers/newlib/libc_syms.o \
./rt-thread/components/libc/compilers/newlib/stdio.o \
./rt-thread/components/libc/compilers/newlib/syscalls.o \
./rt-thread/components/libc/compilers/newlib/time.o 

C_DEPS += \
./rt-thread/components/libc/compilers/newlib/libc.d \
./rt-thread/components/libc/compilers/newlib/libc_syms.d \
./rt-thread/components/libc/compilers/newlib/stdio.d \
./rt-thread/components/libc/compilers/newlib/syscalls.d \
./rt-thread/components/libc/compilers/newlib/time.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/libc/compilers/newlib/%.o: ../rt-thread/components/libc/compilers/newlib/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\applications" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\CubeMX_Config\Core\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\port" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include\config" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\filesystems\devfs" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\spi" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\finsh" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\libc\compilers\newlib" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\common" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\cortex-m7" -include"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

