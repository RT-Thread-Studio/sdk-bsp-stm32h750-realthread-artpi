#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 16
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_HOOK_USING_FUNC_PTR
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 256

/* kservice optimization */

#define RT_KSERVICE_USING_STDLIB

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
#define RT_MEMHEAP_FAST_MODE
#define RT_USING_MEMHEAP_AS_HEAP
#define RT_USING_MEMHEAP_AUTO_BINDING
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart4"
#define RT_VER_NUM 0x40101
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M7

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_MSH
#define RT_USING_FINSH
#define FINSH_USING_MSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_CMD_SIZE 80
#define MSH_USING_BUILT_IN_COMMANDS
#define FINSH_USING_DESCRIPTION
#define FINSH_ARG_MAX 10
#define RT_USING_FAL
#define FAL_DEBUG_CONFIG
#define FAL_DEBUG 1
#define FAL_PART_HAS_TABLE_CFG
#define FAL_USING_SFUD_PORT
#define FAL_USING_NOR_FLASH_DEV_NAME "norflash0"

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_USING_SERIAL
#define RT_USING_SERIAL_V1
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
#define RT_USING_PIN
#define RT_USING_SPI
#define RT_USING_QSPI
#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE
#define RT_SFUD_USING_QSPI
#define RT_SFUD_SPI_MAX_HZ 50000000

/* Using USB */


/* C/C++ and POSIX layer */

#define RT_LIBC_DEFAULT_TIMEZONE 8

/* POSIX (Portable Operating System Interface) layer */


/* Interprocess Communication (IPC) */


/* Socket is in the 'Network' category */


/* Network */


/* Utilities */

#define RT_USING_RYM

/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */

#define PKG_USING_OTA_DOWNLOADER
#define PKG_USING_YMODEM_OTA
#define PKG_USING_OTA_DOWNLOADER_V100

/* security packages */


/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */


/* XML: Extensible Markup Language */


/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */


/* u8g2: a monochrome graphic library */


/* PainterEngine: A cross-platform graphics application framework written in C language */


/* tools packages */


/* system packages */

/* enhanced kernel services */


/* POSIX extension functions */


/* acceleration: Assembly language or algorithmic acceleration packages */


/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */


/* Micrium: Micrium software products porting for RT-Thread */

#define PKG_USING_QBOOT
#define QBOOT_APP_PART_NAME "app"
#define QBOOT_DOWNLOAD_PART_NAME "download"
#define QBOOT_FACTORY_PART_NAME "factory"
#define QBOOT_USING_SHELL
#define QBOOT_SHELL_KEY_CHK_TMO 3
#define QBOOT_USING_OTA_DOWNLOADER
#define QBOOT_USING_PRODUCT_INFO
#define QBOOT_PRODUCT_NAME "ART-Pi QBoot"
#define QBOOT_PRODUCT_VER "v1.00 2021.02.07"
#define QBOOT_PRODUCT_MCU "stm32h750"
#define QBOOT_USING_STATUS_LED
#define QBOOT_STATUS_LED_PIN 47
#define QBOOT_STATUS_LED_LEVEL 1
#define QBOOT_USING_FACTORY_KEY
#define QBOOT_FACTORY_KEY_PIN 116
#define QBOOT_FACTORY_KEY_LEVEL 0
#define QBOOT_FACTORY_KEY_CHK_TMO 10
#define QBOOT_THREAD_STACK_SIZE 4096
#define QBOOT_THREAD_PRIO 5
#define PKG_USING_QBOOT_V105

/* peripheral libraries and drivers */

#define PKG_USING_QLED
#define QLED_TOTAL 5
#define QLED_TIME_UNIT_MS 10
#define QLED_THREAD_NAME "qled"
#define QLED_THREAD_STACK_SIZE 512
#define QLED_THREAD_PRIO 3
#define PKG_USING_QLED_V100

/* AI packages */


/* miscellaneous packages */

/* project laboratory */

/* samples: kernel and components samples */


/* entertainment: terminal games and other interesting software packages */

#define PKG_USING_CRCLIB
#define CRCLIB_USING_CRC8
#define CRC8_USING_CONST_TABLE
#define CRC8_POLY_8C
#define CRC8_POLY 140
#define CRCLIB_USING_CRC16
#define CRC16_USING_CONST_TABLE
#define CRC16_POLY_A001
#define CRC16_POLY 40961
#define CRCLIB_USING_CRC32
#define CRC32_USING_CONST_TABLE
#define CRC32_POLY_EDB88320
#define CRC32_POLY 3988292384
#define PKG_USING_CRCLIB_V100

/* Hardware Drivers Config */

#define SOC_STM32H750XB
#define SOC_SERIES_STM32H7

/* Board extended module */


/* Onboard Peripheral */

#define BSP_USING_USB_TO_USART
#define BSP_USING_SPI_FLASH
#define BSP_USING_QSPI_FLASH

/* On-chip Peripheral */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_USING_UART4
#define BSP_USING_SPI
#define BSP_USING_SPI1
#define BSP_USING_QSPI

/* External Libraries */

#define FIRMWARE_EXEC_USING_QSPI_FLASH
#define RT_STUDIO_BUILT_IN

#endif
