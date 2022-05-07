BSP_ROOT ?= /tmp/sdk-bsp-stm32h750-realthread-artpi/projects/art_pi_mcuboot
RTT_ROOT ?= /tmp/sdk-bsp-stm32h750-realthread-artpi/projects/art_pi_mcuboot/rt-thread

CROSS_COMPILE ?=/opt/arm_tools/gcc-arm-none-eabi-6_2-2016q4/bin/arm-none-eabi-

CFLAGS := -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -std=c99
AFLAGS := -c -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2
LFLAGS := -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T board/linker_scripts/STM32H750XBHx/link.lds
CXXFLAGS := -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g

CPPPATHS :=-I$(BSP_ROOT) \
	-I$(BSP_ROOT)/applications \
	-I$(BSP_ROOT)/board \
	-I$(BSP_ROOT)/board/CubeMX_Config/Core/Inc \
	-I$(BSP_ROOT)/libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Include \
	-I$(BSP_ROOT)/libraries/STM32H7xx_HAL/CMSIS/Include \
	-I$(BSP_ROOT)/libraries/STM32H7xx_HAL/STM32H7xx_HAL_Driver/Inc \
	-I$(BSP_ROOT)/libraries/drivers \
	-I$(BSP_ROOT)/libraries/drivers/include \
	-I$(BSP_ROOT)/libraries/drivers/include/config \
	-I$(BSP_ROOT)/libraries/samples \
	-I$(RTT_ROOT)/components/drivers/include \
	-I$(RTT_ROOT)/components/drivers/spi \
	-I$(RTT_ROOT)/components/finsh \
	-I$(RTT_ROOT)/components/libc/compilers/minilibc \
	-I$(RTT_ROOT)/include \
	-I$(RTT_ROOT)/libcpu/arm/common \
	-I$(RTT_ROOT)/libcpu/arm/cortex-m7 

DEFINES := -DHAVE_CCONFIG_H -DRT_USING_MINILIBC -DSTM32H750xx -DUSE_HAL_DRIVER
