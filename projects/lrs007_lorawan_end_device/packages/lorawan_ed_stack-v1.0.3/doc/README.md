# LoRaWAN-ED-Stack软件包使用说明

# 1 简介
LoRaWAN-ED-Stack软件包是基于RTOS（RT-Thread）实现的LoRaWAN 终端设备侧（**E**nd-**D**evice）的协议栈，可以非常方便进行LoRaWAN协议栈学习、快速搭建基于LoRaWAN通信的原型产品、应用产品等。
LoRaWAN-ED-Stack软件包在LoRaWAN开源协议栈[LoRaMAC-Node](https://github.com/Lora-net/LoRaMac-node)基础上，进一步构建实现。LoRaWAN-ED-Stack使用LoRa-Radio-Driver软件包作为物理层lora芯片的底层驱动，使用Multi-Rtimer软件包或者RT-Thread内核RT_Tick提供MAC层的定时\超时服务。

- LoRaWAN-ED-Stack软件包当前主要功能特点:
    - LoRaWAN技术规范版本（LoRaWAN Specification）
        - LoRaWAN 1.0.x
            - [x] LoRaWAN 1.0.4
                - [x] 当前基于LoRa联盟官方 LoRaMAC-node 4.4.4
        - [ ] LoRaWAN 1.1.x
            - [ ] LoRaMAC-node 5.x
    - 可通过menuconfig配置LoRaWAN MAC层、PHY层、应用层等相关参数
      - LoRaWAN PHY层主要配置参数：
        - LoRaWAN地区规范(region)
            - [x] CN470
            - [x] CN470自定义同频
            - [ ] EU868
            - [ ] AS923
            - [ ] US915
        - [x] 缺省上行速率（LoRaWAN Default Datarate）
        - [x] 缺省信道列表（Channel Mask）
      - LoRaWAN MAC主要配置参数：
        - 设备类型(Device Type)
            - [x] Class A
            - [ ] Class B
            - [x] Class C
        - LoRaWAN数据消息类型(Data Message Type)
            - [x] 确认帧(Confirm Frame)
            - [x] 非确认帧(Unconfirm Frame)
            - [x] LoRaWAN应用端口号（Application Fport）
            - [x] ADR功能
      - LoRaWAN应用层当前提供如下应用实例
        - [x] lorawan-ed-test-shell
            - 用于配置并测试LoRaWAN End-Device Class A \ Class C
      - 功能按需裁剪
        - [x] 可通过宏定义对ClassA、ClassB、Class C、OTAA、ABP等功能进行灵活裁剪
        - [x] 可通过宏定义对日志信息裁剪，详细的协议层日志交互信息，便于前期调试、学习等
            - [x] APS层（入网、通信、用户数据包等）
            - [x] MAC层（MAC请求、MAC命令等）
            - [x] PHY（上下行通信频率、速率等）等的日志输出
      - [x] 支持IDE工具
        - [x] RT-Studio 1.1.4
        - [x] MDK 5
      - 参数掉电保存
        - LoRaWAN的配置参数支持掉电保存
      - 对接LoRaWAN服务器平台
        - [x] 利尔达unicore服务器
            - [x] CN470 & Class A
            - [x] CN470自定义同频 & Class A\C
        - [x] 腾讯云IoT Explorer
            - [x] CN470 & Class A\C

## 1.1 LoRaWAN-ED-Stack软件包组织结构
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603760044028-1261956a-0c9e-41d7-8d79-75f306d36a95.png#align=left&display=inline&height=577&margin=%5Bobject%20Object%5D&name=image.png&originHeight=577&originWidth=864&size=50371&status=done&style=none&width=864)

- app
   - lorawan-ed-test
      - lorawan-ed-test-shell.c
         - End-Device shell测试实现
   - aps 应用支持子层，该文件提供了LoRaWAN End-Device应用层服务接口
      - basic 基础功能
         - lorawan-ed-aps.c
            - 提供应用层状态机，包括入网、通信、认证程序
         - lorawan-ed-certificate.c
            - 提供LoRa联盟认证测试程序
      - advannced 高级功能，当前尚未支持
         - 提供分包、时间同步、组播等功能
   - common
      - utilities.c
         - 提供随机数、拷贝等常用功能函数
      - lorawan-ed-debug.h
         - 用于日志输出控制
   - mac
      - LoRaWAN End-Device Speicificaition的具体实现,主要包括
        - [x] Class A
        - [ ] Class B
        - [x] Class C
      - [x] Mac Command
      - [x] Ctypto  
      - multi-rtimer
         - mac层的定时功能采用multi-rtimer软件包提供的定时服务
         - 若未使能multi-rtimer软件包，则使用系统rt tick提供定时服务。
   - phy
      - LoRaWAN End-Device regional parameters实现，主要包括
        - [x] CN470
        - [x] 自定义CN470S
        - [ ] EU868
        - [ ] AS923
        - [ ] US915
      - lora-radio-driver
         - LoRa芯片驱动使用lora-radio-driver软件包
# 2 LoRaWAN-ED-Stack软件包使用说明
## 2.1 依赖

- RT-Thread 3.0.1+
- LoRa-Radio-Driver
- 可选软件包
   - Multi-Rtimer
      - 若使能multi-rtimer软件包，mac层的定时功能采用multi-rtimer提供的服务，否则使用系统rt tick提供定时功能。
   - easyflash
      - 硬件支持easyflash
      - 用于将LoRaWAN-ED的配置参数保存到Flash，实现掉电存储
- 可选内核组件
   - ulog组件，开启后，可得到更详细的日志提示信息等
      - lorawan-ed-stack内部使用ulog接口，使能ulog，可看到更多调试输出信息
      - lorawan-ed-test-shell.c使用ulog接口，用于打印测试信息
```
 RT-Thread Components --->
    Utiliess --->
        [*] Enable ulog
            [*] Enable ISR log.
```

## 2.2 获取软件包
 使能LoRaWAN-ED-Stack软件包，根据实际需要，配置LoRaWAN End-Device的相关参数:

1. 选择LoRaWAN Specification版本，当前已测试版本1.0.X
1. 配置LoRaWAN-ED mac参数，设置设备类型(Class A\Class B\Class C）、激活类型(OTAA\ABP)、数据消息类型(unConfirm\Confirm)、ADR使能\关闭等
1. 配置LoRaWAN-ED phy参数，设置频段(Regional)、工作信道（Channel mask）、缺省上行速率等
1. 配置LoRaWAN-ED Debug信息，根据需求，使能不同层的日志输出
1. 选择LoRaWAN End-Device测试示例
   1. LoRaWAN-ED-Test-Shell.c 
      1. Class A\Class C入网、数据通信、LoRaWAN上下行网络链路质量测试等 
### 2.2.1 MDK 5开发环境
使用 LoRaWAN-ED-Stack软件包，需要在 RT-Thread 的包管理中选中它，具体路径如下：
```
RT-Thread online packages --->
    IoT - Internet of things --->
       [*] lorawan_ed_stack: LoRaWAN End-Device Stack.                                                                               │ │
             Select LoRaWAN Specification (v1.0.X)  --->                                                                             │ │
             LoRaWAN Network Attribute (Public Network)  --->                                                                        │ │
             LoRaWAN Alliance Certificate Test (Disable)  --->                                                                       │ │
             Select LoRaWAN-ED Mac Parameters  --->                                                                                  │ │
             Select LoRaWAN-ED Phy Parameters  --->                                                                                  │ │
             Select LoRaWAN-ED Debug  --->                                                                                           │ │
             Select LoRaWAN-ED-Stack Example  --->                                                                                   │ │
             Version (latest)  --->
```
### 2.2.2 RT-Thread Studio开发环境
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603352034888-b1ad6f37-ac0d-41be-9b10-466b3aa476dc.png#align=left&display=inline&height=368&margin=%5Bobject%20Object%5D&name=image.png&originHeight=368&originWidth=1129&size=41800&status=done&style=none&width=1129)
# 3 应用实例
## 3.1 lorawan-ed-test-shell
lorawan-ed-test-shell提供了常用的shell命令，用来读写LoRaWAN End-Device相关MAC、PHY等参数，并进行LoRaWAN入网、Class A通信等测试验证。
当前支持的命令如下
<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1604024396415-be877105-b1e2-4f2e-ad4b-555cbb62378b.png#align=left&display=inline&height=355&margin=%5Bobject%20Object%5D&name=image.png&originHeight=355&originWidth=1064&size=56622&status=done&style=none&width=1064)
| 序号 | 命令类型 | finish命令 | 说明 |
| --- | --- | --- | --- |
| 1 | 参数设置与读取 | lorawan deveui <para1> | 设置\读取DevEUI<br />- 有<para1>，设置DevEUI的值,16进制，8个字节<br />- 无 para1，读取当前DevEUI的值<br /> |
| 2 | 参数设置与读取 | lorawan appeui <para1> | 设置\读取AppEUI<br />- 有<para1>，设置AppEUI的值,16进制，8个字节<br />- 无 para1，读取当前AppEUI的值<br /> |
| 3 | 参数设置与读取 | lorawan appkey <para1> | 设置\读取AppKey<br />- 有<para1>，设置AppKey的值,16进制，16个字节<br />- 无 para1，读取当前AppKey的值<br /> |
| 4 | 参数设置与读取 | lorawan devaddr <para1> | 设置\读取DevAddr<br />- 有<para1>，设置DevAddr的值,16进制，4个字节,仅适用于ABP<br />- 无 para1，读取当前DevAddr的值<br /> |
| 5 | 参数设置与读取 | lorawan appskey <para1> | 设置\读取AppSKey<br />- 有<para1>，设置AppSKey的值,16进制，16个字节,仅适用于ABP<br />- 无 para1，读取当前AppSKey的值<br /> |
| 6 | 参数设置与读取 | lorawan nwkskey <para1> | 设置\读取NwkSKey(NwkSEncKey)<br />- 有<para1>，设置NwkSKey的值,16进制，16个字节,仅适用于ABP<br />- 无 para1，读取当前NwkSKey的值<br /> |
| 7 | 参数设置与读取 | lorawan class <para1> | 设置\读取 Class类型<br />- 有<para1>，设置Class的值,1个字节<br />   - 0 - Class A<br />   - 1 - Class B<br />   - 2 - Class C<br />- 无 para1，读取当前Class的值<br /> |
| 8 | 参数设置与读取 | lorawan confirm <para1> | 设置\读取 数据传输类型 Confirm\Unconfirm<br />- 有<para1>，设置数据传输类型的值,1个字节<br />   - 0 - UnConfirm<br />   - 1 - Confirm<br />- 无 para1，读取当前数据传输类型的值<br /> |
| 9 | 参数设置与读取 | lorawan activation <para1> | 设置\读取 设备激活方式<br />- 有<para1>，设置激活方式的值,1个字节<br />   - 0 - OTAA<br />   - 1 - ABP<br />- 无 para1，读取当前激活方式的值<br /> |
| 10 | 入网 | lorawan join <nbtrials><interval> | 启动入网<br />- nbtrials：单次最大入网重试次数<br />   - 0 - 停止入网<br />   - 非0 -  入网重试次数<br />- interval：入网包之间的发送间隔，单位：秒，最小周期8s<br /> |
| 11 | 数据通信 | lorawan ping <nbtrials><interval> | LoRaWAN网络上下行链路质量测试<br />在设备入网后，执行该指令，LoRaWAN终端发送指定数量的Link Check测试数据包，测试完成后，shell输出当前上下行无线链路质量结果<br />- nbtrials：每次ping测试，发送的数据包总数<br />   - 缺省nbtrials = 10<br />- interval：ping数据包之间的发送间隔，单位：秒<br /> |
| 12 | 数据通信 | lorawan tx <mode><cfm><port><len><data> | 发送Class A数据包<br />- mode 发送模式<br />   - 0 - 停止周期性发送<br />   - 1 - 立即发送一次<br />   - 2 ~ 1500 - 按次数发送，执行该指令后，设备发送指定次数的数据包后停止，发送间隔为10s<br />   - ＞1500 - 按周期发送，单位ms，执行该指令后，设备周期性发送<br />- cfm 数据消息类型<br />   - 0 - 非确认帧<br />   - 1 - 确认帧<br />- port 应用端口号<br />   - 1~223<br />- len - 数据包长度<br />- data - 自定义发送数据包{x1,x2,x3...}，16进制格式<br /> |
| 13 | 保存 | lorawan save <type> | 保存配置信息到Flash（需要硬件支持easyflash），<type>缺省情况下，默认保存为<cfg><br />- type 保存类型<br />   - dev - 设备身份信息(DevEUI、AppEUI、AppKey等)<br />   - cfg - LoRaWAN工作参数(Class Type、OTAA\ABP等)<br /> |
| 14 | 恢复出厂设置 | lorawan factory  | 恢复LoRaWAN工作参数为出厂值 |
    
测试示例：
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603407358137-619d136b-2d3b-4c0b-9d1b-cc7da562cb56.png#align=left&display=inline&height=386&margin=%5Bobject%20Object%5D&name=image.png&originHeight=386&originWidth=1089&size=61389&status=done&style=none&width=1089)
OTAA入网测试（标准CN470-OTAA-ClassA）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603407412072-49ea8b01-e9bb-41e5-ae82-34fe79952639.png#align=left&display=inline&height=443&margin=%5Bobject%20Object%5D&name=image.png&originHeight=443&originWidth=1164&size=153163&status=done&style=none&width=1164)
OTAA入网测试（CN470自定义-OTAA-ClassA）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603410298124-6d8d3035-1a74-4d39-92db-92faaf5377f0.png#align=left&display=inline&height=851&margin=%5Bobject%20Object%5D&name=image.png&originHeight=851&originWidth=1312&size=440741&status=done&style=none&width=1312)
非确认帧通信测试（CN470自定义-OTAA-ClassA）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603411743124-8ec7c180-22dc-4b56-a0dc-3a0e4652120e.png#align=left&display=inline&height=804&margin=%5Bobject%20Object%5D&name=image.png&originHeight=804&originWidth=1501&size=186279&status=done&style=none&width=1501)
确认帧测试(标准CN470-OTAA-ClassA)
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603410266966-8205af56-b133-4ca2-8aa2-09ab29392b68.png#align=left&display=inline&height=1231&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1231&originWidth=1500&size=254888&status=done&style=none&width=1500)
lorawan ping测试（标准CN470-OTAA-ClassA）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1603758595350-031389bc-efd5-4ad7-9790-fedf9cb53ed8.png#align=left&display=inline&height=1440&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1440&originWidth=1525&size=261428&status=done&style=none&width=1525)
入网与服务器主动下发下行数据测试（标准CN470-OTAA-Class C）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/253586/1604030596575-0d02f799-6cb8-4979-a865-66cefc2cd66f.png#align=left&display=inline&height=2022&margin=%5Bobject%20Object%5D&name=image.png&originHeight=2022&originWidth=1489&size=384752&status=done&style=none&width=1489)<br />ABP直接通信测试（CN470自定义-ABP-Class A-掉电参数）
## 3.2 应用层调用说明


1. 初始化lorawan协议栈
```c
/* Configure the LoRaWAN End-Device Stack*/
lorawan_ed_stack_init(&lorawan_ed_user_app_callback, &lorawan_ed_init_params);
```

2. 启动入网
```c
LoRaMacStatus_t lorawan_ed_start_join_network( void )
```

3. 发送上行数据包
```c
bool lorawan_ed_send(lorawan_ed_appdata_t* AppData)
```

4. 接收下行数据
```c
static void lorawan_ed_receive_message(lorawan_ed_appdata_t *app_data)
```
# 4 版本更新历史 Change log

## 4.1 LoRaWAN-ED-Stack 1.0.0 pre-released 20.10.27
- LoRaWAN-ED-Stack1.0.0 基于LoRaMac-node 4.4.4 released版本
    - 支持lorawan regional parameters 1.0.3
    - 协议栈本身的实现最大程度保持LoRaMAC-node保持一致，便于后续持续更新
    - 修复下行Buffer潜在的溢出风险漏洞 （腾讯安全团队）
- 支持lorawan-ed-test-shell命令快速进行LoRaWAN End-Device参数配置与功能验证、测试等
- 支持menuconfig进行lorawan协议参数的灵活配置
- 支持LoRaWAN地区规范(region)
    - [x] CN470
    - [x] CN470自定义同频

# 5 参考

- LoRa Allicance LoRaWAN技术规范
   - [https://lora-alliance.org/resource-hub/lorawanr-specification-v103](https://lora-alliance.org/resource-hub/lorawanr-specification-v103)
   - [https://lora-alliance.org/resource-hub/rp2-101-lorawanr-regional-parameters-0](https://lora-alliance.org/resource-hub/rp2-101-lorawanr-regional-parameters-0)
- LoRaMac-node官方仓库
   - [https://github.com/Lora-net/LoRaMac-node](https://github.com/Lora-net/LoRaMac-node)
- LoRa-Radio-Driver仓库
   - [https://github.com/Forest-Rain/lora-radio-driver](https://github.com/Forest-Rain/lora-radio-driver)
- Multi-Rtimer仓库
   - [https://github.com/Forest-Rain/multi-rtimer](https://github.com/Forest-Rain/multi-rtimer)
- RT-Thread官方仓库
   - [https://github.com/RT-Thread/rt-thread](https://github.com/RT-Thread/rt-thread)



