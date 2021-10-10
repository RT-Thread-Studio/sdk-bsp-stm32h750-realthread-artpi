# LoRa-Radio-Driver软件包使用说明

# 1 简介
LoRa-Radio-Driver软件包是基于RTOS( RT-Thread ) 实现的LoRa Tranceiver芯片的驱动文件，可用于快速搭建基于LoRa等通信的应用产品。
LoRa-Radio-Driver软件包在LoRaWAN开源协议栈[LoRaMAC-Node中的radio](https://github.com/Forest-Rain/lora-radio-driver/blob/master/doc)基础上，进一步封装实现。
> LoRaMac\Radio
> [https://github.com/Lora-net/LoRaMac-node/tree/master/src/radio](https://github.com/Lora-net/LoRaMac-node/tree/master/src/radio)


- 主要特点:
   - 当前支持LoRa Transceiver（sx126x\sx127x ）
      - 支持调制方式
         - [x] LoRa
         - [ ] FSK
   - 可通过EVN工具menuconfig直接定义LoRa模块的对外接口，降低入门门槛
     - 支持使用引脚号来定义GPIO
     - 支持使用引脚名来定义GPIO
   - 提供常用实例代码，可用于射频性能测试、空口数据包监听、双向通信测试等
   - 可作为phy层对接到LoRaWAN End-Device协议栈
   - 当前测试的LoRa 模块\芯片
      - LoRa Transceiver (SPI)
         - SX126X (SX1262\ASR6500S\LLCC68\SX1268..)
            - SX1268
               - [x] [LSD4RF-2R717N40 (CN470频段)](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)
            - SX1262
               - [x] ASR6500S
               - [x] [LSD4RF-2R822N30 (868/915MHz频段)](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)
            - LLCC68
            - LR1110
         - SX127X (SX1272\SX1276\SX1278..)
            - SX1278
               - [x] [LSD4RF-2F717N30(CN470频段)](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)
               - [x] [Ra-01](http://wiki.ai-thinker.com/lora/man)
         - [ ] SX1276
      - LoRa SIP\SoC
   - 当前测试的MCU平台
      - LoRa Radio Driver当前功能主要在STM32L平台测试通过，未来计划将适配更多的MCU平台（华大MCU、nRF、BK）
         - [x] STM32L0系列
         - [x] STM32L4系列
         - [x] STM32H7系列
   - 当前支持的RTOS
      - [x] RT-Thread
      - [ ] RT-Thread Nano
   - 当前测试的IDE
      - [x] MDK5.29
      - [x] RT-Thread Studio 1.1.3
# 2 LoRa Radio Driver 软件包组织结构
![image.png](https://github.com/Forest-Rain/lora-radio-driver/raw/master/doc/pics/01_lora-radio-driver_pkgs_struction.png)

- lora-radio
   - sx126x
      - lora-radio-sx126x.c
         - 对外提供了上层访问接口实现
      - lora-spi-sx126x.c 
         - sx126x芯片的spi读写接口实现，独立于MCU平台
            - [x] rt_device
            - [ ] SPI裸机方式
      - sx126x.c
         - lora芯片sx126x底层驱动
   - sx127x
      - lora-radio-sx127x.c 
         - 对外提供了上层访问接口
      - lora-spi-sx127x.c
         - sx127x芯片的spi读写接口实现，独立于MCU平台
            - [x] rt_device
            - [ ] SPI裸机方式
      - sx127x.c
         - lora芯片sx127x底层驱动
   - common
      - lora-radio-timer.c
         - 提供了lora-radio所需的定时服务接口，用于发送与接收超时等，基于RT-Thread内核rt_timer实现
            - 注意这种方式提供的定时最小颗粒度取决于系统tick RT_TICK_PER_SECOND
            - 注:如果使能了Multi-Rtimer软件包，则优先使用Multi-Rtimer提供定时\超时服务
   - include
      - lora-radio.h
         - 上层服务接口
      - lora-radio-debug.h
         - 根据需要使能输出lora-radio不同层级的调试信息
      - lora-radio-rtos-config.h
         - rtos适配层，选择，当前默认为RT-Thread
         - 未来支持RT-Thread-Nano、以及其他RTOS....
- samples
   - lora radio driver示例文件
      - lora-radio-test-shell
         - shell示例，主要实现了射频性能测试、空口数据包监听、双向通信测试等shell命令，便于日常测试
- port
   - 主要包含当前在不同MCU平台下支持的lora模块，lora-module文件夹中的xxxx-borad.c包含了与LoRa模块直接相关的主要硬件接口配置：
      - lora-module
         - stm32_adapter 
            - lora-board-spi.c
               - STM32平台的SPI外设初始化等通用接口
            - LSD4RF-2F717N30 （SX1278 LoRa模块）
            - LSD4RF-2R717N40 （SX1268 LoRa模块）
            - LSD4RF-2R822N30 （SX1262 LoRa模块）
            - Ra-01 （SX1278 LoRa模块）
               - xxxx-borad.c
                  - LoRa模块功率输出方式（PA\RFO...）
                  - LoRa模块的RF高频开关控制方式（TXE、RXE、无..）
                  - LoRa模块的DIO口（DIO0、DIO1、DIO2....）
                  - LoRa模块的工作频率限制等
         - xxx_adapter
            - 其他mcu平台下的硬件接口实现
# 3 LoRa Radio Driver软件包使用说明
## 3.1 依赖

- SPI外设——用户需根据实际MCU平台，自定义LoRa模块实际所需要使用的SPI外设
   - 选择SPI外设 
```
  Hardware Drivers Config --->
     On-chip Peripheral Drivers --->
         [*] Enable SPI  --->
                 --- Enable SPI
                 [ ]   Enable SPI1
                 [ ]   Enable SPI2
                 [ ]   Enable SPI3
                 [ ]   Enable SPI4
                 [ ]   Enable SPI5
```
   - 在bsp\目标板XX\board\Kconfig增加如下定义
```c
  menuconfig BSP_USING_SPI
       bool "Enable SPI"
       select RT_USING_SPI

       if BSP_USING_SPI
         config BSP_USING_SPI1
            bool "Enable SPI1"
            default n
            if BSP_USING_SPI1
              config BSP_SPI1_RX_USING_DMA
                bool "Enable SPI1 RX DMA"
                default n
              config BSP_SPI1_TX_USING_DMA
                bool "Enable SPI1 TX DMA"
                default n
            endif
            
            # 根据实际需要，增加其他BSP_USING_SPI2、BSP_USING_SPI3...
        endif
```

- 定时服务——用于提供射频通信所需的定时\超时服务，目前支持以下两种方式，二选一
   - 内核 SOFT_TIMER
      - 若未选用Multi-Rtimer软件包，则默认采用内核的rt_timer来提供定时服务(lora-radio-timer.c)
      - 注意检测是否**开启RT-Thread内核的SOFT_TIMER**
   - M[ulti-Rtimer](https://github.com/Forest-Rain/multi-rtimer)软件包
      - 若使能multi-rtimer，lora-radio-driver优先使用multi-rtimer提供定时\超时服务。 
> 注：如果应用在工业温度范围、时间精度要求高(us\ms级别)的场景，建议使用multi-rtimer，并设置RTC时钟源为外部32768晶振，否则可能会出现下行丢包的情况。

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] multi_rtimer: a real-time and low power software timer module. --->
                Version (latest)  --->
                multi_rtimer options --->
                    [] multi_rtimer demo example
```

- 可选内核组件
   - ulog组件——用于打印日志信息
      - 使能ulog
         - ulog缓存大小设置≥ 128 Byte
         - lora-raido-driver内部可看到更多LoRa底层的调试信息
         - lora-radio-test-shell.c使用ulog接口，用于打印调试信息、原始16进制数据等
      - 如果没有使用ulog，默认使用rt_kprintf来实现信息输出功能
```
RT-Thread Components --->
   Utiliess --->
       [*] Enable ulog
           [*] Enable ISR log.
```

## 3.2 获取软件包
使用 lora-radio-driver 软件包，需要在 RT-Thread 的包管理中选中它，具体路径如下：
```c
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] lora_radio_driver: lora chipset(sx126x\sx127x.)driver. --->
            Select LoRa Radio Object Type (LoRa Radio Single-Instance)
                (lora-radio0)Setup LoRa Radio Device Name       
                (spi3)  Setup LoRa Radio Spi Name (Define BSP_USING_SPIx in [Target Platform]\Board\Kconfig)
                        Select LoRa Chip Type (LoRa Transceiver [SX126X])  --->
                        Select Supported LoRa Module [SX126X]  --->
                [ ]     Enable LoRa Radio Debug
                        Select LoRa Radio Driver Sample --->  
                    Version (latest)  --->
```

1. Select LoRa Chip \ LoRa Module
   1. "Setup LoRa Radio Device Name"
      1. 设置LoRa Radio设备名称，缺省为"lora-radio0"
   2. "Setup LoRa Radio Spi Name"
      1. 设置LoRa Radio Spi名称
      1. 若在 [Target Platform]\Board\Kconfig提前设定好所使用的BSP_USING_SPIx，则会自动配置
   3. "Select LoRa Radio Single-Instance"
      1. 选择为单实例对象，当前只支持单个lora设备
   4. "Select LoRa Chip Type"
      1. 选择实际使用的LoRa芯片类型
         - 当前支持 SX126X、SX127x Transceiver
   5. "Select Supported LoRa Module"
      1. 选择lora模块，根据实际使用的MCU硬件平台与lora模块，配置关联的GPIO引脚等功能选项
         1. 设定LoRa模块的GPIO口（比如 RESET、NSS、BUSY、DIO1、TXE、RXE...）
            - " Select LoRa Chip GPIO by Pin Number  "
               - 支持使用引脚号来定义GPIO，比如 输入 10 代表 A10 
            - "Select LoRa Chip GPIO by Pin Name"
               - 支持使用引脚名来定义GPIO，比如 输入 A10 代表引脚GPIOA的PIN10脚 (STM32)
2. Select LoRa Radio Driver Sample
   1. 根据实际情况，可选择测试示例
## 3.3 新增LoRa模块
在 lora-radio-driver\ports\lora-module文件下，参考已有模板，根据实际需要增加新的mcu平台适配文件、新的lora模块驱动文件xxxx-board.c


# 4 使用示例

## 4.1 硬件测试平台
当前所使用的硬件测试平台如下所示
| 序号 | 硬件平台 | MCU | LoRa模块 | 主要用户接口 |
| --- | --- | --- | --- | --- |
| 1 | LSD4RF-TEST2002  | STM32L476VG | [LSD4RF-2R717N40](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)<br />[ ( SX1268 )](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87) | <br />- 用户接口定义<br />   - VCC  - 3.3V<br />   - GND<br />   - SCK    - PC10 (SPI3)<br />   - MISO  - PC11 (SPI3)<br />   - MOSI  - PC12 (SPI3)<br />   - NSS    - PA15<br />   - RESET - PA7<br />   - DIO0  - PB1<br />   - BUSY - PB2<br />   - RFSW1 - PB0<br />   - RFSW2 - PC5<br />- 射频开关TX trace<br />   - TX: RFSW1 = 1 , RFSW2 = 0<br />   - TX: RFSW1 = 0 , RFSW2 = 1<br /> |
| 2 | LSD4RF-TEST2002  | STM32L476VG | [LSD4RF-2F717N30](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)<br />[ ( SX1278 )](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87) | <br />- 用户接口定义<br />   - VCC   - 3.3V<br />   - GND<br />   - SCK    - PC10 (SPI3)<br />   - MISO  - PC11 (SPI3)<br />   - MOSI  - PC12 (SPI3)<br />   - NSS    - PB6<br />   - RESET - PA7<br />   - DIO0  - PB1<br />   - DIO1  - PC4<br />   - DIO2  - PB2<br />   - DIO3  - NC<br />   - DIO4  - NC<br />   - RFSW1 - PB0<br />   - RFSW2 - PC5<br />- 射频开关TX trace<br />   - TX: RFSW1 = 1 , RFSW2 = 0<br />   - TX: RFSW1 = 0 , RFSW2 = 1<br /> |
| 3 | Nucleo-L476RG | STM32L476RG | [Ra-01](http://wiki.ai-thinker.com/lora/man)<br />(RT-thread LoRa Adruino扩展板V1) | <br />- 用户接口定义<br />   - VCC    - 3.3V<br />   - GND<br />   - SCK    - PA5(SPI1)<br />   - MISO  - PA6(SPI1)<br />   - MOSI  - PA7(SPI1)<br />   - NSS    - PB6<br />   - RESET - PC7<br />   - DIO0  - PA9<br />   - DIO1  - PA8<br /> |
| 4 | ART-Pi  | STM32H750XB | [LSD4RF-2R717N40](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)<br />[ ( SX1268 )](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87) | <br />- 用户接口定义<br />   - VCC  - 3.3V<br />   - GND<br />   - SCK    - PI1 (SPI2)<br />   - MISO  - PI2 (SPI2)<br />   - MOSI  - PI3 (SPI2)<br />   - NSS    - PI0<br />   - RESET - PA15<br />   - DIO1  - PG7<br />   - BUSY - PH15<br />   - RFSW1 - PH14<br />   - RFSW2 - PH13<br />- 射频开关TX trace<br />   - TX: RFSW1 = 1 , RFSW2 = 0<br />   - TX: RFSW1 = 0 , RFSW2 = 1<br /> |
| 5 | ART-Pi  | STM32H750XB | [LSD4RF-2R822N30](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87)<br />[ ( SX1262 )](http://bbs.lierda.com/forum.php?mod=viewthread&tid=87) | |
## 4.2 Shell测试命令
若使能 [* ] LoRa Radio Test Shell，则可以通过shell（finish）命令直接进行LoRa相关测试
```c
[*] Enable LoRa Radio Test Shell                                                                                              │ │
     Select the RF frequency (Region CN470)  --->                                                                            │ │
     Select RF Modem (Modem LoRa)  --->
```
![image.png](https://github.com/Forest-Rain/lora-radio-driver/raw/master/doc/pics/02_lora-radio-test-shell-cmdlist.png)

| 序号 | finish命令 | 说明 |
| --- | --- | --- |
| 1 | lora probe | 测试lora设备(SPI)访问是否正常 |
| 2 | lora cw <para1> <para2> | \<para1\>:频点，单位Hz<br>\<para2\>:功率，单位dBm|
| 3 | lora ping <para1> <para2><para3> | \<para1\> : 主机\从机<br>-m 主机<br>-s 从机<br> \<para2\>: 发送数据包个数<br>\<para3\>: pingc测试数据包长度,最大支持255Byte |
| 4 | lora rx  | 接收数据包，同时以16进制格式与ASCII码显示数据内容 |
| 5 | lora config <para1> <para2> | \<para1\>:radio参数，字符表示<br/>  freq 表示频率，单位Hz<br/>  power 表示发射功率，单位dbm<br/>  sf 表示扩频因子，有效值: 7~12<br/>  bw表示带宽，有效值: 0 (125kHz)、1 (250KHz)、2 (500KHz)<br/>  public表示同步字，有效值: 0 （sync = 0x12), 1  (sync = 0x34)<br/>  iq 表示iq反转，有效值: 0 (iq不反转)，1 (iq反转)<br/>\<para2\>:radio参数的具体值 |

![image.png](https://github.com/Forest-Rain/lora-radio-driver/raw/master/doc/pics/03_lora-ping_SX1278-SX1268-TRX-test.png)
lora ping 双向通信测试示例(SX1278 <-> SX1268)
![image.png](https://github.com/Forest-Rain/lora-radio-driver/raw/master/doc/pics/04_lora-rx_sniffer-test.png)
lora rx 单向接收(监听)lora数据包测试示例 (SX1278 <- 或-> SX1268)

## 4.3 应用层调用说明
用户层调用可以参考如下步骤

1. 定义射频DIO中断服务回调函数
```c

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

```
2. 调用lora-radio初始化
```c
void main(void)
{
    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    if(Radio.Init(&RadioEvents))
    {
        Radio.SetPublicNetwork( false );
        lora_chip_initialized = true;
    }
    //.....
}
```

3. 配置射频通信参数
```c
{
    Radio.SetChannel( lora_radio_test_paras.frequency );

    if( lora_radio_test_paras.modem == MODEM_LORA )
    {
        Radio.SetTxConfig( MODEM_LORA, lora_radio_test_paras.txpower, 0, lora_radio_test_paras.bw,
                          lora_radio_test_paras.sf, lora_radio_test_paras.cr,
                          LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE,
                          true, 0, 0, LORA_IQ_INVERSION_ON_DISABLE, 3000 );

        Radio.SetRxConfig( MODEM_LORA, lora_radio_test_paras.bw, lora_radio_test_paras.sf,
                          lora_radio_test_paras.cr, 0, LORA_PREAMBLE_LENGTH,
                          LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON_DISABLE,
                          0, true, 0, 0, LORA_IQ_INVERSION_ON_DISABLE, true );
    }
}
```

4. 数据发送
```c
Radio.Send( Buffer, len );
```
5. 数据接收
```c
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    BufferSize = size;
    rt_memcpy( Buffer, payload, BufferSize );
    rssi_value = rssi;
    snr_value = snr;
    // .....
}
```
# 5 版本更新历史

- V1.0.0 版本 2020-06-20
   - 基于lorawan4.4.2 release版本的radio
   - 主体功能实现基于STM32平台
      - 支持SX126x、SX127x系列芯片
         - 测试LoRa芯片支持LSD4RF-2R717N40(SX1268)、SX1278、ASR6500S @ [**zyk6271**](https://github.com/zyk6271)
   - 支持基于RT-Thread内核rt_timer的lora-radio-timer接口@ [**AnswerInTheWind** ](https://github.com/AnswerInTheWind)
   - 优化日志换行功能、sx126x.c的multi-rtimer.h包含问题@[**zyk6271**](https://github.com/zyk6271)
- V1.1.0 版本 2020-08-30
   - 完善用户使用指南
   - .lora-radio-driver软件包
      - 新增日志输出选择 lora-radio-debug.h，可以按需开启调试日志，也可以用于适配不同日志输出方式
      - 新增rtos适配选择 lora-radio-rtos-config.h，便于未来适配RT-Thread-Nano、不同的RTOS平台
      - lora-radio(sx126x\sx127x)
         - 同步更新到lorawan4.4.4 release版本的radio
            - sx126x更新 SX126xSetLoRaSymbNumTimeout（同步到loramac-node-master）
            - sx126x更新 RadioRandom 与 SX126xGetRandom
            - 更新 RadioIrqProcess
            - 更新RadioTimeOnAir
         - RadioIrqProcess 增加 临界区保护，优先完成RF中断回调服务函数，防止出现硬件异常
      - 调整lora-radio-driver软件包架构，便于未来适配不同的MCU平台
         - port目录下新增mcu平台适配层，如stm32_adapter
   - lora-radio-test-shell
      - 修复 PHY CRC Error后，没有重新进入接收问题
      - lora ping命令
         - 新增发送空口数据包的TOA时间显示
         - 新增主机侧接收到数据包后，seqno显示
   - [Kconfig](https://github.com/Forest-Rain/packages/blob/master/peripherals/lora_radio_driver)
      - 更新[lora-radio-driver\Kconfig](https://github.com/Forest-Rain/packages) 软件包配置文件
         - 区分单实例(单lora模块)与多实例（多lora模块）情况，目前支持单实例
         - 移除了Kconfig中对BSP_USING_SPIx的直接定义，BSP_USING_SPIx定义调整到[Target Platform]\Board\Kconfig)
         - 重命名宏定义REGION_X为PHY_REGION_X(如REGION_CN470 -> PHY_REGION_CN470)，以便与LoRaWAN协议栈中缺省REGION_X共存
- V1.1.2 版本 2020-10-12
   - 修复Ra-01未同步与v1.1.1更新导致的问题
   - 优化 drv_gpio.h使用，兼容RT-Thread Studio
   - 优化 lora-radio-test-shell.c 功能
     - 新增接收超时时间设置
 - V1.2 版本 2020-10-14
    - 新增硬件测试平台
      - ART-Pi+LSD4RF-2F717N30(SX1268)模块平台 (470~510MHz频段)
      - ART-Pi+LSD4RF-2R717N40(SX1268)模块平台 (470~510MHz频段)
      - ART-Pi+LSD4RF-2R822N30(SX1262)模块平台 (868/915MHz频段)
 - V1.4.0 版本 2021-04-25
   - 重设计lora config命令，便于快速配置单个radio参数
   - ping数据包长度最大支持255Byte，可通过shell自定义ping测试数据包长度
   - shell新增加iq version、public network参数设置
   - 使用LORA_RADIO_DEBUG_LOG代替rt_kprintf
# 6 问题和建议
如果有什么问题或者建议欢迎提交 [Issue](https://github.com/Forest-Rain/lora-radio-driver/issues) 进行讨论。
