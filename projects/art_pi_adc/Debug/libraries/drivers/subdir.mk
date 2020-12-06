################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/drivers/drv_adc.c \
../libraries/drivers/drv_common.c \
../libraries/drivers/drv_gpio.c \
../libraries/drivers/drv_lcd.c \
../libraries/drivers/drv_sdio.c \
../libraries/drivers/drv_sdram.c \
../libraries/drivers/drv_spi.c \
../libraries/drivers/drv_usart.c \
../libraries/drivers/drv_wlan.c 

OBJS += \
./libraries/drivers/drv_adc.o \
./libraries/drivers/drv_common.o \
./libraries/drivers/drv_gpio.o \
./libraries/drivers/drv_lcd.o \
./libraries/drivers/drv_sdio.o \
./libraries/drivers/drv_sdram.o \
./libraries/drivers/drv_spi.o \
./libraries/drivers/drv_usart.o \
./libraries/drivers/drv_wlan.o 

C_DEPS += \
./libraries/drivers/drv_adc.d \
./libraries/drivers/drv_common.d \
./libraries/drivers/drv_gpio.d \
./libraries/drivers/drv_lcd.d \
./libraries/drivers/drv_sdio.d \
./libraries/drivers/drv_sdram.d \
./libraries/drivers/drv_spi.d \
./libraries/drivers/drv_usart.d \
./libraries/drivers/drv_wlan.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/drivers/%.o: ../libraries/drivers/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\applications" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\CubeMX_Config\Core\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board\port" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\board" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include\config" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\libraries\drivers" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\filesystems\devfs" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\dfs\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\drivers\spi" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\finsh" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\components\libc\compilers\newlib" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\include" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\common" -I"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rt-thread\libcpu\arm\cortex-m7" -include"D:\Software\RT-ThreadStudio\workspace\art_pi_adc\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

