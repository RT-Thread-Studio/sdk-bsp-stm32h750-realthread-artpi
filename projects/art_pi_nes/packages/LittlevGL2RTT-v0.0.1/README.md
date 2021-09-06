# LittlevGL2RTT #

[![Build Status](https://travis-ci.org/liu2guang/LittlevGL2RTT.svg?branch=master)](https://travis-ci.org/liu2guang/LittlevGL2RTT)
[![release](https://img.shields.io/badge/Release-v0.0.1-orange.svg)](https://github.com/liu2guang/LittlevGL2RTT/releases)

LittlevGL2RTT 是在RTThread3.0版本以上做的LittlevGL中间框架层, 目的是无缝嵌入RTT的LCD框架, 无需开发者去移植. 如果您觉得该库看得顺眼舒服，请捐赠颗小星星. 小星星就是更新的动力!!! 

感谢LittlevGL作者的开源GUI库. 原库请移步到 [https://github.com/littlevgl/lvgl](https://github.com/littlevgl/lvgl "lvgl"). 

----
![littlevgl2rtt交流群群二维码](https://i.imgur.com/COnvJ4A.png)
----

## 1. 效果图

![效果图](https://i.imgur.com/hdq3RhV.gif)

## 2. 安装LittlevGL2RTT

目前littlevgl2rtt库已经添加到rtt官方pkgs包中, 可以直接在menuconfig在线包中直接使能. 

1. 在env中运行menuconfig. 
2. 进入RT-Thread online packages -> system packages目录. 
3. 开启LittlevGL2RTT, 选择version为lateset最新版本, 配置分辨率为你的显示屏分辨率, 然后开启demo并保存退出menuconfig.
4. 执行pkgs --update更新LittlevGL2RTT包到你的bsp下面. 
5. 执行scons/scons --target=xxx, 进行编译生成工程, 下载运行.
 
![安装流程](https://i.imgur.com/fojc8Ie.gif)

## 3. 卸载LittlevGL2RTT

1. 在env中运行menuconfig. 
2. 进入RT-Thread online packages -> system packages目录. 
3. 关闭LittlevGL2RTT.
4. 执行pkgs --update, 并输入`Y`表示同意删除pkg包文件. 
5. 执行scons/scons --target=xxx, 进行编译生成工程, 下载运行.

![卸载流程](https://i.imgur.com/yAeXejV.gif)

## 4. 注意事项

因为LittlevGL使用了匿名结构体, 所以需要在MDK中需要添加编译参数: **--gnu -g -W**. 

## 5. 开发进度

1. 开发环境主要是RT1050 fire的板子(lcd为800*480, 输入设备为GT911), 和qemu环境(800*480, 输入设备为鼠标). 
2. 输入接口已经完成适配，目前在野火1050板子上测试通过, 效果稳定[update].
3. 目前只测试过16bit模式.
4. 关于更多的lvgl的配置导出到menuconfig欢迎在issues里面提出. 
