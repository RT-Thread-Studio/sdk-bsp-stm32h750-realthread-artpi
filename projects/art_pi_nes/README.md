# NES模拟器例程

## 简介

本例程主要功能是使用多媒体扩展板，运行nes模拟器，通过触摸屏选择游戏并试玩。

使用前，请将nes游戏文件(游戏文件在`packages/nes-latest/games`目录下)复制到sd卡或者flash文件系统中，本例程在RT-Thread Studio中测试，请暂不要使用keil（nes的mapper相关cpp文件会报错，缺少相关变量定义等）

## 硬件说明
ART-Pi核心板及多媒体扩展版

## 软件说明

### 注意事项
    `/libraries/drivers/include/lcd_spi_port.h`中LCD_HOR_SCREEN用于定义横屏显示，LCD_FULL_COLOR用于lcd初始的颜色背景（黑色）

### 软件包更改部分
    `/packages/LittlevGL2RTT-latest/littlevgl2rtt.c`文件(将lvgl的ARGB8888转换为RGB888)
    `/packages/ft6236-latest/src/ft6236.c`文件（更改为多点触摸）

### 源码介绍
    `/applications/font`目录存放字库文件
    `/applications/img`目录存放游戏操作按钮的图片文件
    `/applications/nes`目录为nes软件包需要的接口文件，实际接口不在其内只有调用函数操作
    `/applications/main.c`为闪灯测试
    `/applications/touch.c`为触摸驱动，默认情况触摸接入到了lvgl中，当进入nes游戏界面时，将其脱离，原因是lvgl不支持多点触摸
    `/applications/ui_control.c`为nes游戏界面的触摸操作按键绘制及检测，并介入到nes模拟器接口中
    `/applications/ui_home.c`用于初始上电的RT-Thread图标显示界面
    `/applications/ui_nes_list.c`主要用于刷新文件列表，该部分可能会存在问题（点击非文件夹目录，存在需多次退出才能返回上一层目录；文件列表刷新可能会丢失部分项（与lvgl的优先级有关））
    `/applications/ui_nes.c`用于调用nes模拟器加载游戏文件及实现屏幕的接口函数（nes的图像数据格式为RGB565）


## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

正常运行后，会显示RT-Thread图标，点击图标即可进入文件系统列表，然后进入存放nes游戏文件夹内，单击游戏即可试玩。

## 注意事项

本例程目前暂未支持声音输出，敬请谅解
同时keil编译时，nes软件包下的cpp文件会报错，缺失定义

## 联系方式
邮箱：		ghazi.gq@hotmail.com
gitee地址：	https://gitee.com/Ghazi_gq
github地址：https://github.com/Ghazigq

