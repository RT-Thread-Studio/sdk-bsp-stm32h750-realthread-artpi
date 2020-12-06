################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/src/clock.c \
../rt-thread/src/components.c \
../rt-thread/src/device.c \
../rt-thread/src/idle.c \
../rt-thread/src/ipc.c \
../rt-thread/src/irq.c \
../rt-thread/src/kservice.c \
../rt-thread/src/memheap.c \
../rt-thread/src/mempool.c \
../rt-thread/src/object.c \
../rt-thread/src/scheduler.c \
../rt-thread/src/signal.c \
../rt-thread/src/thread.c \
../rt-thread/src/timer.c 

OBJS += \
./rt-thread/src/clock.o \
./rt-thread/src/components.o \
./rt-thread/src/device.o \
./rt-thread/src/idle.o \
./rt-thread/src/ipc.o \
./rt-thread/src/irq.o \
./rt-thread/src/kservice.o \
./rt-thread/src/memheap.o \
./rt-thread/src/mempool.o \
./rt-thread/src/object.o \
./rt-thread/src/scheduler.o \
./rt-thread/src/signal.o \
./rt-thread/src/thread.o \
./rt-thread/src/timer.o 

C_DEPS += \
./rt-thread/src/clock.d \
./rt-thread/src/components.d \
./rt-thread/src/device.d \
./rt-thread/src/idle.d \
./rt-thread/src/ipc.d \
./rt-thread/src/irq.d \
./rt-thread/src/kservice.d \
./rt-thread/src/memheap.d \
./rt-thread/src/mempool.d \
./rt-thread/src/object.d \
./rt-thread/src/scheduler.d \
./rt-thread/src/signal.d \
./rt-thread/src/thread.d \
./rt-thread/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/src/%.o: ../rt-thread/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\applications" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\CubeMX_Config\Core\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\port" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include\config" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\filesystems\devfs" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\spi" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\finsh" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\libc\compilers\newlib" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\common" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\cortex-m7" -include"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

