# ART-Pi 开发手册

## 简介

本文将详细介绍 ART-Pi 开发流程，让用户掌握 ART-Pi 开发环境的搭建。

## ART-Pi SDK 下载指南

本章节将列举几种获取 sdk 的渠道。

### 1. 在 RT-Thread Studio 里直接下载 sdk

  RT-Thread Studio 提供的一站式的开发流程，如下图是 ART-Pi SDK 在  RT-Thread Studio 里的下载流程：

![sdk_manager](./figures/sdk_manager.png)

### 2. 在 GitHub 里直接下载 sdk

  ART-Pi 是 RT-Thread 官方发起的一个开源社区活动，它是开源的。所以你同样可以在各大开源代码托管网站里下载到 ART-Pi 的 sdk 源码。
  在 GitHub 网站里，我们 ART-Pi 官方的 sdk 源码地址是： [链接](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi)

  如下图所示从 GitHub 下载 ART-Pi SDK 流程：

![github_sdk](./figures/github_sdk.png)

## RT-Thread Studio 工程开发指南

如下图所示创建一个工程：

![](./figures/new_prj_1.png)

填写工程名称；选择 Base On Board；选择 Example；

![](./figures/new_prj_2.png)

双击 RT-Thread Setting 打开图形配置界面。

![](./figures/new_prj_4.png)

打开 HardWare 选项卡；选择 Enable UART4；（可选步骤，这里只是简单展示如何添加驱动。）

![](./figures/new_prj_5.png)

如图所示进行编译；

![](./figures/new_prj_3.png)

在下载烧录程序前确认下 debuger 配置情况：

![](./figures/debuger.png)

点击下载按钮，下载烧录程序：

![](./figures/download.png)

## MDK 工程开发指南

首先进入工程目录。如：sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_blink_led

如图所示以管理员身份打开 PowerShell：

![](./figures/powershell.png)

依次输入如下三个命令：

```c
cmd
mklink /D rt-thread ..\..\rt-thread
mklink /D libraries ..\..\libraries
```

![](./figures/mklink.png)

然后在该目录下，使用 RT-Thread 官方提供的 env 工具按照 rt-thread 开发的正常流程创建 MDK 工程。

输入 `scons --target=mdk5` 命令生成 MDK 工程。

![](./figures/scons_mdk.png)

![](./figures/open_mdk.png)