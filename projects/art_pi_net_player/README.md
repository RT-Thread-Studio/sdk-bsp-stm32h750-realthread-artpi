# mp3播放例程

## 简介

本例程主要功能是从文件系统或者网络获得mp3数据并播放。

界面采用的是`柿饼UI`，解码库用的`Helix`，网络音频数据从`网易云`获得。

**本例程只支持 RT-Studio，不支持keil。**

**界面有点简陋，不是柿饼的锅，是我刚刚接触柿饼不熟悉的缘故，有兴趣的小伙伴可以一起来鼓捣。**

## 硬件说明

本例程基于多媒体扩展板实现：

**外设：**

-SAI2：产生I2S时序，播放音频

-I2C2：触摸接口

-I2C3：音频芯片 wm8988控制接口

-UART4：调试端口

-ETH：网络

-SPI1：flash

-SPI2：屏幕驱动接口

-SDMMC1：SD卡通讯接口

-SDMMC2：AP6212通讯接口

**软件包：**

-fal

-ft6236

-helix

-webclient

-EasyFlash

-ezXML

-fastlz

-littlefs

-mbedtls



## 软件说明

本例程源代码位于 `/projects/art_pi_net_player`中

## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

体验本例程前需要先下载`art_pi_bootloader`和`art_pi_wifi`工程

   -`bootloader`     负责启动例程。

   -` art_pi_wifi `   负责wifi相关的配置，如上传固件等。

### 运行效果

正常运行后，蓝色 LED 会周期性闪烁

如果你的文件系统中没用UI相关的文件，开机后是**不能显示界面**的。此时需要去下载`player`的柿饼的UI文件。

​		下载地址：[ART-Pi 播放器UI文件 (github.com)](https://github.com/liuduanfei/player)

需要将工程目录下的player文件夹整体拷贝到  /flash 中。

**注意**是 player下的

​				--player（*）这个文件夹。

​				--temp

​                --list.ui

​                   ......

有两种拷贝方式：

1. 使用 `ftp` 工具，例程中已经使能了 `ftp服务器`，可以用 `FileZilla`工具将文件上传到`/flash`中
2. 使用 `sd卡`，先将UI文件放到sd卡中，然后用命令 `cp /sdcard/player /flash` 拷贝文件。

拷贝完后，需要重启系统，就能看到播放界面了。此时还没有从网络上获取播放歌曲的信息，所以还**不能播放**。

需要**配置网络**，例程中支持了wifi。可以用下面的命令进行联网：

 `wifi join  <WiFi名称> <wifi密码>`

确认**网络连接成功**后，插上耳机，点击屏幕播放按键，就可以体验歌曲了（第一次只能按播放按键获取播放信息，所以有点延迟）。

如果需要播放**本地文件**中的mp3，需要在SD卡中music文件夹下放mp3文件（支持递归扫描`.mp3`文件），切换数据源之后就可以播放文件系统中的文件了。



## 再次寻找志同道合的小伙伴一起学习柿饼UI，做一个漂亮的界面，完善该demo，有兴趣的可以联系我。



## 注意事项

本例程用到了helix解码库，且网络数据源为网易云，仅供学习交流。



## 联系人信息

维护人:

- [liuduanfei]([liuduanfei (liuduanfei) (github.com)](https://github.com/liuduanfei)), 邮箱：[939763442@qq.com](mailto:939763442@qq.com)