# AT device #

## 1. 简介 ##


AT device 软件包是由 RT-Thread AT 组件针对不同 AT 设备的移植文件和示例代码组成，目前支持的 AT 设备有：ESP8266、ESP32、M26、MC20、RW007、MW31、SIM800C、W60X 、SIM76XX、A9/A9G、BC26 、AIR720、ME3616、M6315、BC28、EC200X系列设备等，目前上述设备都完成对 `AT socket` 功能的移植，及设备通过 AT 命令实现标准 socket 编程接口，完成 socket 通讯的功能，具体功能介绍可参考 [《RT-Thread 编程指南》](https://www.rt-thread.org/document/site/programming-manual/at/at/)AT 命令章节 。 

### 1.1. 目录结构 ###

| 名称 | 说明 |
| ---- | ---- |
| src | AT device 实现源码目录 |
| inc | AT device 头文件目录 |
| sample | 不同设备示例文件目录 |
| class | 不同设备针对 AT 组件的移植适配目录 |
| class/esp8266 | ESP8266 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/esp32   | ESP32 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/rw007 | RW007 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/sim800c | SIM800C 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/sim76xx | SIM76XX 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/m26 | M26/MC20 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/ec20 | EC20 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/mw31 | MW31 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/w60x | W60X 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/a9g | A9G 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/bc26 | bc26 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/air720 | air720 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/me3616 | me3616 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/m6315 | m6315 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/bc28 | bc28 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/ec200x | EC200T、EC200S 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/n21 | N21 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
| class/n58 | N58 设备针对 AT 组件的移植目录，实现 AT Socket 功能 |
### 1.2 许可证 ###

at_device package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖 ###

- RT_Thread 4.0.2+
- RT_Thread AT 组件  1.3.0+
- RT_Thread SAL 组件
- RT-Thread netdev 组件

## 2. 获取方式 ##

AT device 软件包是对 AT 组件和 AT socket 功能的移植，需开启 AT 组件库和 AT socket 功能来获取 AT device 软件包。

**版本号说明**

AT device 软件包目前已经发布多个版本，各个版本之间选项配置方式和其对应的系统版本有所不同，下面主要列出当前可使用的软件包版本信息：

- **V1.2.0**：适用于 RT-Thread 版本小于 V3.1.3，AT 组件版本等于 V1.0.0；
- **V1.3.0**：适用于 RT-Thread 版本小于 V3.1.3，AT 组件版本等于 V1.1.0；
- **V1.4.0**：适用于 RT-Thread 版本小于 V3.1.3或等于 V4.0.0， AT 组件版本等于 V1.2.0；
- **V1.5.0**：适用于 RT-Thread 版本小于 V3.1.3 或等于 V4.0.0， AT 组件版本等于 V1.2.0；
- **V1.6.0**：适用于 RT-Thread 版本等于 V3.1.3 或等于 V4.0.1， AT 组件版本等于 V1.2.0；
- **V2.0.0/V2.0.1**：适用于 RT-Thread 版本大于 V4.0.1 或者大于 3.1.3， AT 组件版本等于 V1.3.0；
- **laster**：只适用于 RT-Thread 版本大于 V4.0.1 或者大于 3.1.3， AT 组件版本等于 V1.3.0；

>  上述 版本判断在 menuconfig 中自动完成，at_device 软件包选择版本时会根据当前系统环境给出最佳版本支持，这里版本介绍作为运行环境参考。

针对不同的版本号，在 ENV 中的选项配置也有所不同，主要分为如下几部分：

**V1.X.X 版本配置选项介绍**

开启 AT device 软件包，该版本只支持**同时开启一种 AT 设备**， 配置选项具体如下所示：

    RT-Thread online packages  --->
         IoT - internet of things  --->
            -*- AT DEVICE: RT-Thread AT component porting or samples for different device  
            [ ]   Enable at device init by thread
                  AT socket device modules (Not selected, please select)  --->    
                  Version (V1.6.0)  --->

- **Enable at device init by thread**: 配置开启设备网络初始化是否通过创建线程完成；
- **AT socket device modules**: AT 设备选择，目前支持 RW007、ESP8266、M26/MC20、EC20、SIM800C、SIM76XX、A9/A9G、BC26 、air720、ME3616、M6315、BC28、EC200X等设备；
- **Version**: 下载软件包版本；

**V2.X.X (laster) 版本配置选项介绍**

开启 AT device 软件包，该版本支持**同时开启多种 AT 设备**配置选项具体如下所示：

```c
RT-Thread online packages  --->
     IoT - internet of things  --->
        -*- AT DEVICE: RT-Thread AT component porting or samples for different device
        [*]   Quectel M26/MC20  --->
          [*]   Enable initialize by thread
          [*]   Enable sample
          (-1)    Power pin
          (-1)    Power status pin
          (uart3) AT client device name
          (512)   The maximum length of receive line buffer
        [ ]   Quectel EC20  --->
        [ ]   Espressif ESP32  --->
        [*]   Espressif ESP8266  --->
          [*]   Enable initialize by thread
          [*]   Enable sample
          (realthread) WIFI ssid
          (12345678) WIFI password
          (uart2) AT client device name
          (512)   The maximum length of receive line buffer
        [ ]   Realthread RW007  --->
        [ ]   SIMCom SIM800C  --->
        [ ]   SIMCom SIM76XX  --->
        [ ]   Notion MW31  --->
        [ ]   WinnerMicro W60X  --->
        [ ]   AiThink A9/A9G  --->
        [ ]   Quectel BC26  --->
        [ ]   Luat air720  --->
        [ ]   GOSUNCN ME3616  --->
        [ ]   ChinaMobile M6315  --->
        [ ]   Quectel BC28  --->
        [ ]   Quectel ec200x  --->
        [ ]   Neoway N21  --->
        [ ]   Neoway N58  --->
        Version (latest)  --->
```

- **Quectel M26/MC20**：开启 M20/MC20（2G 模块）设备支持；
  - **Enable initialize by thread**：开启使用线程进行设备初始化功能（非阻塞模式初始化）；
  - **Enable sample**：开启示例代码，该示例代码中有对示例设备的注册；
  - **Power pin**：配置该示例设备上电引脚；
  - **Power status pin**：配置该示例设备上电状态引脚；
  - **AT client device name**：配置该示例设备使用的串口设备名称；
  - **The maximum length of receive line buffer**：配置该示例设备最大一行接收的数据长度；
- **Quectel EC20**：开启 EC20（4G 模块）设备支持；
- **Espressif ESP8266**：开启 ESP8266 （WIFI 模块）设备支持；
  - **Enable initialize by thread**：开启使用线程进行设备初始化功能（非阻塞模式初始化）；
  - **Enable sample**：开启示例代码，该示例代码中有对示例设备的注册；
  - **WIFI ssid**：配置该示例设备连接的 WIFI 用户名；
  - **WIFI password**：配置该示例设备连接的 WIFI 密码；
  - **AT client device name**：配置该示例设备使用的串口设备名称；
  - **The maximum length of receive line buffer**：配置该示例设备最大一行接收的数据长度；
- **Espressif ESP32**：开启 ESP32 （WIFI 模块）设备支持；
- **Realthread RW007**：开启 RW007 （WIFI 模块）设备支持；
- **SIMCom SIM800C**：开启 SIM800C （2G 模块）设备支持；
- **SIMCom SIM76XX**：开启 SIM76XX （4G 模块）设备支持； 
- **Notion MW31**：开启 MW31 （WIFI 模块）设备支持； 
- **WinnerMicro W60X**：开启 W60X （WIFI 模块）设备支持；
- **AiThink A9/A9G**：开启 A9/A9G （2G 模块）设备支持；
- **Quectel BC26**：开启 BC26（NB-IOT 模块）设备支持；
- **Luat Air720**：开启 air720（4g 模块）设备支持；
- **GOSUNCN ME3616**：开启 ME3616（NB-IOT 模块）设备支持；
- **ChinaMobile M6315**：开启 M6315 （2G 模块）设备支持；
- **Quectel BC28**：开启 BC28（NB-IoT 模块）设备支持；
- **Quectel EC200X**：开启 EC200T、EC200S（4G 模块）设备支持；
- **Neoway N21**：开启 N21（NB-IoT 模块）设备支持；
- **Neoway N58**：开启 N58（4G 模块）设备支持；
- **Version** 下载软件包版本；

上面配置选项以 2G 模块和 WIFI 模块选项为例，介绍了`V2.X.X` 版本 AT device 软件包配置方式，如下几点值得注意：

- `V2.X.X` 版本支持同时开启多个 AT 设备，可以在 FinSH 中通过 `ifocnfig` 命令查看开启的设备信息；
- `V2.X.X` 版本设备需要注册之后才可使用，目前在 samples 目录文件中完成设备注册，用户也可以在应用层自定义设备注册。
-  `Power pin` 和 `Power status pin` 等引脚选项根据具体设备硬件连接情况配置，如果不使用硬件上电功能，可以配置为 `-1`；
- 一个 AT 设备对应一个串口名称，及每个设备配置的`AT client device name` 应该都不相同。

**AT 组件相关配置选项介绍**

选中 AT device 软件包并开启相关设备支持之后，会默认选中 AT 组件的客户端功能，下面是 AT 组件配置选项，

```c
RT-Thread Components  --->
    Network  --->
        AT commands  --->
    [ ]   Enable debug log output
    [ ]   Enable AT commands server 
    -*-   Enable AT commands client
    (1)     The maximum number of supported clients
    -*-     Enable BSD Socket API support by AT commnads
    [*]     Enable CLI(Command-Line Interface) for AT commands
    [ ]     Enable print RAW format AT command communication data
    (128)   The maximum lenght of AT Commonds buffe
```

其中和 AT device 软件包相关的配置选项：

- **The maximum number of supported clients**：最大支持的同时开启的 AT 客户端数量（AT device 软件包中选中多个设备需要配置该选项为对应数值）；
- **Enable BSD Socket API support by AT commnads**：开启 AT Socket 功能支持，选中 AT device 软件包默认选中该选项；
- **The maximum lenght of AT Commonds buffe**：最大支持的发送命令数据的长度。

## 3. 注意事项  ##

- AT device 软件包适配的模块暂时不支持作为 TCP Server 完成服务器相关操作（如 accept 等）；
- AT device 软件包默认设备类型为未选择，使用时需要指定使用设备型号；
- `laster` 版本支持多个选中多个 AT 设备接入实现 AT Socket 功能，`V1.X.X` 版本只支持单个 AT 设备接入。
- AT device 软件包目前多个版本主要用于适配 AT 组件和系统的改动，推荐使用最新版本  RT-Thread 系统，并在 menuconfig 选项中选择 `latest` 版本；

## 4. 相关文档

- [AT 组件编程指南](https://www.rt-thread.org/document/site/programming-manual/at/at/)
- [应用 AT 组件连接 ESP8266 模块](https://www.rt-thread.org/document/site/application-note/components/at/an0014-at-client/)

## 5. 联系方式

* 维护：RT-Thread 开发团队及社区开发者
* 主页：https://github.com/RT-Thread-packages/at_device
