# ART-Pi bootloader 生成与下载手册



## 简介：

本文将介绍 ART-Pi  bootloader 的几种生成方式以及下载方式，可用于bootloader缺失后的恢复，包括以下内容：

-   生成 ART-Pi bootloader
-   使用 RT-Thread Studio 生成并下载 bootloader
-   使用 MDK 生成并下载 bootloader
-   使用 ST-LINK 工具下载现有的 bootloader hex 文件



## 获取 ART-Pi SDK

依据 ART-Pi 开发手册 获取 ART-Pi SDK 。



## 使用 RT-Thread Studio 生成并下载 bootloader

### 1. 创建 bootloader 例程项目并编译

在 RT-Thread 包管理器中获取 ART-Pi SDK后。

点击文件 - 新建 - RT-Thread 项目，按照下图选择 art_pi_bootloader 例程：

![](figures/bootloader_rttstdio_new_prj.png)

项目成功创建后，直接点击编译。

![](figures/bootloader_rttstdio_build.png)



### 2 . 下载 bootloader

**注意：**由于 bootloader 是下载到 STM32H750XBH6 的片上 flash 中，而不是下载到外部 flash。 因此在 **debugger配置** 界面的 **Download** 选项页中 **不需要** 做其他配置，保持默认即可（这里与应用程序下载有区别）。

<img src="figures/bootloader_rttstdio_download.png" style="zoom: 80%;" />



用 Tpye-C 数据线 将 ART-Pi 的 USB-DBG 接口与 PC 相连， 接着点击下载按钮烧录 bootloader 到芯片中。

![](figures/bootloader_rttstdio_flash.png)



## 使用 MDK 生成并下载 bootloader

依据开发手册在 [ART-Pi SDK仓库](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi) 下载 SDK。

### 1.创建art_pi_bootloader项目

进入工程目录，如：sdk-bsp-stm32h750-realthread-artpi-master\projects\art_pi_bootloader。

双击执行 mklinks.dat，分别为 `rt-thread` 及 `libraries` 文件创建符号链接；生成以下两个文件夹则表示连接成功。

![](figures/bootloader_MDK_config.png)

 

使用 ENV 工具执行 scons --target=mdk5 更新 MDK5工程文件，并且点击工程打开。

![](figures/bootloader_MDK_open_prj.png)



### 2.编译并下载

先构建，后下载。

![](figures/bootloader_MDK_build&download.png)

**注意：**如果下载失败，可以尝试先擦除芯片 Flash 后进行烧写。



## 使用 ST-LINK 工具下载现有的 bootloader hex文件

还可以使用 ST-LINK 工具下载现有的 bootloader hex文件，首先要准备 hex 文件。如果已经有现成的 hex 文件则不需要生成，直接进行第2步下载 hex 文件。

sdk中包含了预先生成的 bootloader hex 文件，路径为 `sdk-bsp-stm32h750-realthread-artpi/tools/bootloader/ART-Pi_bootloader.hex` 

### 1.hex文件生成过程

**使用 RT-Thread Stdio:** 

创建完art_pi_bootloader工程后，在编译之前要修改项目属性配置，当前默认会生成 bin 二进制文件，修改为生成 hex 文件（Ouput file format）。

![](figures/bootloader_rttstdio_hex.png)

应用并关闭后点击编译按钮即可。

 hex 文件会在项目编译时自动生成，一般生成在工程目录下 Debug 文件夹中。



**使用 MDK ：**

打开工程后，点击魔术棒进行目标选项配置：

![](figures/bootloader_MDK_hex.png)

点击构建后，在工程目录下的build文件夹中会自动生成 hex 文件。



### 2.下载 Hex 文件

ST-Link 下载工具使用 STM32 ST-LINK Utility 。

用 Tpye-C 数据线 将 ART-Pi 的 USB-DBG 接口与 PC 相连， 点击 Target - Settings 设置一下接口。

![](figures/bootloader_stlink_target_setting.png)



点击 OK 后可以看到以下信息，且连接成功（如未自动连接，则点击Target - Connect，或点击工具栏第三个图标）。

![](figures/bootloader_stlink_connect.png)

并且此时 ART-Pi 上 ST-LINK 指示灯开始闪烁，表示已连接。

点击 Binary File ，选择刚刚编译生成的 hex 文件。

![](figures/bootloader_stlink_select_hex.png)

点击 Program Verify - Start 即可开始下载烧录。

![](figures/bootloader_stlink_download_hex.png)

下载完成后断开 ST-LINK 连接即可。



## Bootloader 下载完成

通过前面任意方式，Bootloader 下载完成后，连接终端。复位ART-Pi， 即可看到 bootloader 打印信息。

![](figures/bootloader_download_success.png)