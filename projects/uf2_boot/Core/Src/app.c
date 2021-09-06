
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "usb_device.h"

#include "w25q64jv.h"
#include "w25q128jv.h"
#include "uf2.h"
#include "flash.h"

#define APPLICATION_ADDRESS 0x90000000UL
typedef void (*appFun)(void);
appFun App;
static void GoToApp(void)
{
    __set_PRIMASK(1);
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    for (int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }
    __set_PRIMASK(0);
    uint32_t *vector_table = (uint32_t *)APPLICATION_ADDRESS;
    uint32_t stack_point = vector_table[0];
    uint32_t entry_point = vector_table[1];
    App = (appFun)entry_point;
    __set_MSP(stack_point);
    //__set_CONTROL(0);
    App();
}

void user_main(void)
{
    uint8_t w25q64jv_device_id[2];
    uint8_t w25q64jv_id_number[12];

    printf("ART-Pi Bootloader(0x%p). \r\n", user_main);
    QSPI_W25Q64JV_Reset();
    QSPI_W25Q64JV_DeviceID(w25q64jv_device_id);
    printf("Manufacturer / Device ID: 0x%x 0x%x \r\n", w25q64jv_device_id[0], w25q64jv_device_id[1]);
    QSPI_W25Q64JV_IDNumber(w25q64jv_id_number);
    printf("Unique ID Number: ");
    for (int i = 4; i < 12; i++)
    {
        printf("0x%02x ", w25q64jv_id_number[i]);
    }
    printf(" \r\n");

    uint8_t w25q128jv_device_id[2];
    uint8_t w25q128jv_id_number[12];

    SPI_W25Q128JV_Reset();
    SPI_W25Q128JV_DeviceID(w25q128jv_device_id);
    printf("Manufacturer / Device ID: 0x%x 0x%x \r\n", w25q128jv_device_id[0], w25q128jv_device_id[1]);
    SPI_W25Q128JV_IDNumber(w25q128jv_id_number);
    printf("Unique ID Number: ");
    for (int i = 4; i < 12; i++)
    {
        printf("0x%02x ", w25q128jv_id_number[i]);
    }
    printf(" \r\n");

    if (HAL_GPIO_ReadPin(BOOT_KEY_GPIO_Port, BOOT_KEY_Pin) == GPIO_PIN_RESET)
    {
        uf2_init();
        flash_init();
        MX_USB_DEVICE_Init();
    }
    else
    {
        //SCB_DisableICache();
        //SCB_DisableDCache();
        QSPI_W25Q64JV_EnableMemoryMappedMode();
        HAL_GPIO_DeInit(BOOT_KEY_GPIO_Port, BOOT_KEY_Pin);
        HAL_UART_DeInit(&huart4);
        GoToApp();
    }
}
