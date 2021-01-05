# LoRaWAN-ED-Stack Shell例程

## 简介

LoRaWAN-ED-Stack Shell示例程序主要功能是通过串口shell命令来控制LoRaWAN End Device实现LoRaWAN Class A\C入网、链路测试、通信测试等功能。

该例程基于ART-Pi与LoRa多功能扩展板(LRS007)实现，建立在lora-radio例程之上，使用了470M LoRa模块(LSD4RF-2R717N40[SX1268])。

[LSD4RF-2R717N40[SX1268]](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)主要技术参数：

- LoRa芯片：SX1268
- 频段范围：470~510M
- 发射性能：~105mA@ 22dBm(max)
- 接收性能:  ~6mA, -124dBm@SF7&BW125
- 晶振类型：TXCO
- 通信接口：SPI

## 硬件说明
<img src="./figures/LoRa-Shield_LRS007_RF_A_SCH_SX126x.png" alt="LED 连接单片机引脚" style="zoom: 50%;" />
如上图所示，LRS007扩展板的M6位号当前默认贴装470M LoRa模块(LSD4RF-2R717N40[SX1268])，其采用硬件SPI接口(SPI4)与ART-Pi主板连接。

## 软件说明

基于ART-Pi的lorawan-ed-stack软件包使用参考如下

![lorawan-ed-stack软件包使用说明](./figures/lorawan-ed-stack-manual-for-art-pi.gif)

lorawan-ed-stack的测试代码当前采用的是 projects/lrs007_lorawan_end_device/packages/lorawan_ed_stack-v1.0.3/samples/lorawan-ed-shell
具体使用方式可参考[《lorawan-ed-stack软件包使用说明》](https://github.com/Forest-Rain/lorawan-ed-stack/tree/master/doc)


## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

![lorawan-ed-stack通信测试](./figures/lorawan-ed-stack-testl-llcc68-cn470s.gif)。

## 注意事项

1. LoRaWAN Class A通信对时序要求相对较高，当前示例使能multi-rtimer软件包来提供定时\超时服务（STM32平台）。也可根据实际情况，使用了内核的rt_timer来提供定时服务。
2. 如果使能ulog进行日志输出，需要开启“Enable ISR log”，为了保证日志输出完整，日志最大长度建议设置≥384
3. 根据实际使用的LoRaWAN服务器平台，修改设备的DevEUI、AppEUI、AppKey参数
4. 当前示例主要测试CN470、CN470自主定义两种Regional方式

