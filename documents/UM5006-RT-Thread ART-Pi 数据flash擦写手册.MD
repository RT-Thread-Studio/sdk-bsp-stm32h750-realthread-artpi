# UM5006-RT-Thread ART-Pi 数据flash擦写手册



## ART-Pi 数据FLASH(W25Q128JV)简介

ART-Pi除了配备一颗W25Q64JV用于存放程序外,同时配备了另一颗W25Q128JV用于存放WIFI固件,蓝牙固件,等其它数据.官方建议这颗W25Q128JV的空间划分如下:

| 分区名     | 起始地址        | 分区大小 | 用途说明            |
| ---------- | --------------- | -------- | ------------------- |
| wifi_image | 0               | 512KB    | 保存wifi固件        |
| bt_image   | 512 * 1024      | 512KB    | 保存bt固件          |
| download   | 1 * 1024 * 1024 | 2048KB   | 固件下载分区        |
| easyflash  | 3 * 1024 * 1024 | 1024KB   | easyflash参数保存区 |
| filesystem | 4 * 1024 * 1024 | 12MB     | 文件系统分区        |

ART-Pi出厂时数据flash中已经刷入相应的数据信息,但有时我们可能意外破坏或擦除上面的数据,此时就要重新写入数据,为此编写了数据flash的stldr下载算法,通过STM32CubeProgrammer加载下载算法可方便读写数据flash.

## STM32CubeProgrammer下载算法加载

将文件ART-Pi SDK-->debug-->stldr-->ART-Pi_W25Q128JV.stldr拷贝到STM32CubeProgrammer的外部flash算法目录(如:C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader)下

![cubeprog](.\figures\cubeprog.gif)

![cubeprog2](.\figures\cubeprog2.gif)

## STM32CubeProgrammer擦写数据flash

本下载算法可以一次性擦除整块flash,也可以按64KB块大小分别擦除.

![cubeprog3](.\figures\cubeprog3.gif)

下面以刷入wifi固件为例做演示(记住flash分区划分wifi固件起始地址是0,数据flash的在STM32CubeProgrammer中的地址是0x70000000,所以0x70000000+0(分区偏移)就是我们要写入的起始地址),STM32CubeProgrammer只能打开bin等后缀名的文件,所以要将wifi固件文件名后缀改成bin.

![cubeprog4](.\figures\cubeprog4.gif)







