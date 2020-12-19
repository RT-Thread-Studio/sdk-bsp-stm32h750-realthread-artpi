# LoRaWAN-ED-Stack软件包 简介
LoRaWAN-ED-Stack软件包是基于RTOS（RT-Thread）实现的LoRaWAN 终端设备侧（**E**nd-**D**evice）的协议栈，可以非常方便进行LoRaWAN协议栈学习、快速搭建基于LoRaWAN通信的原型产品、应用产品等。
LoRaWAN-ED-Stack软件包在LoRaWAN开源协议栈[LoRaMAC-Node](https://github.com/Lora-net/LoRaMac-node)基础上，进一步构建实现。LoRaWAN-ED-Stack使用LoRa-Radio-Driver软件包作为物理层lora芯片的底层驱动，使用Multi-Rtimer软件包或者RT-Thread内核RT_Tick提供MAC层的定时\超时服务。

更多详细信息请查看[LoRaWAN-ED-Stack/doc](https://github.com/Forest-Rain/lorawan-ed-stack/tree/master/doc)
