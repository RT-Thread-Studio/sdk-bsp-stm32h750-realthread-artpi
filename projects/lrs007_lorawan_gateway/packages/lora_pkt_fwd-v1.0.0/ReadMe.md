# lora-pkt-fwd软件包 简介
lora-pkt-fwd软件包基于RTOS（RT-Thread）与LoRa网关模块(eg:SX1302模块..)实现了Semtech定义的GWMP协议，进而实现LoRaWAN数据包通过UDP转发到LoRaWAN服务器。lora-pkt-fwd软件包建立在UDP基础之上，可以用于构建符合Semtech定义的第一代LoRaWAN网关。

lora-pkt-fwd软件包使用lora-gw-driver软件包提供的lora数据包的发送与接收服务，当前主要支持SX1302芯片。

更多详细信息请查看[lora-pkt-fwd/docs](https://github.com/Forest-Rain/lora-pkt-fwd/tree/master/docs)

