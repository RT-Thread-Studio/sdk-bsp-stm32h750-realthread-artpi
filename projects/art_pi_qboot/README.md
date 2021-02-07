# BootLoader 例程

## 简介

本例程主要的功能是让程序从 `0x08000000`跳转到`0x90000000`。
STM32H750 的片上 ROM 大小为128K，无法满足一般的程序开发，必须使用 XIP 的方式运行程序。所以需要使用 BootLoader 来跳转到 XIP(0x90000000) 地址。
这个例程也可以作为您开发更多高级 bootloader 功能的基础工程。

## 硬件说明

<img src="./figures\qspi_flash.png" alt="qspi_flash" style="zoom:50%;" />

如上图所示，QSPI_FLASH 与单片机的 QSPI 外设引脚相连。

## 软件说明

源代码位于 `/projects/art_pi_bootloader/applications/main.c` 中。

在 main 函数中，实现了跳转。

```
int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    W25QXX_Init();

    W25Q_Memory_Mapped_Enable();

    SCB_DisableICache();
    SCB_DisableDCache();

    SysTick->CTRL = 0;

    JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    JumpToApplication();

    return RT_EOK;
}
```



## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

上电之后会执行 bootloader 程序之后就会跳转执行 QSPI FLASH 中的可执行程序

## 注意事项

1. 如果 QSPI FLASH 中没有可执行程序，那么 MCU 在执行跳转指令后就会停止运行。
