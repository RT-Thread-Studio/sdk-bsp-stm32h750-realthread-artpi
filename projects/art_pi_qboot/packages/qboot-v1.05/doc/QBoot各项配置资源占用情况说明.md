# Quick bootloader v1.0.0 各功能模块资源使用情况列表
---
### 注：
### - 表中flash和ram数值的单位为字节。
### - 其中`(+4096)`表示单独打开此项时会增加4096字节ram使用，如果多个带有`(+4096)`的项被同时打开时，总共只增加4096字节，`(+1024)`同理。
### - 表中所列ram占用不包括线程栈的使用。
### - 最小功能时flash使用约5.3k，ram使用约4.1k；全功能时flash使用约37.4k，ram使用约17.7k。
### - 列表所示是使用RT-Thread Studio编译器，在优化选项-Os下，编译所得测试结果。
---

|功能模块|选项宏|flash|ram|说明|使用推荐|
|----|----|----|----|----|---|
|Qboot|PKG_USING_QBOOT|5392+1156|4192|组件核心功能模块,1156是v1.02版本加入检验app合法性增加的代码尺寸|必备|
|Syswatch|QBOOT_USING_SYSWATCH|2812|100|使用系统看守组件，以保障系统运行安全|安全第一，推荐使用|
|Factory key|QBOOT_USING_FACTORY_KEY|80|0|使用恢复出厂按键功能|有按键时，推荐使用|
|Status led|QBOOT_USING_STATUS_LED|980|20|使用运行状态指示功能|方便识别状态，推荐使用|
|Product code|QBOOT_USING_PRODUCT_CODE|120|0|使用产品码鉴别功能|防止非法固件升级，推荐使用|
|AES|QBOOT_USING_AES|11568|296 (+4096)|使用AES解密功能|资源占用略大，视需求使用|
|gzip(laster)|QBOOT_USING_GZIP|9972|8268 (+4096)|使用gzip解压缩功能|压缩率较好，资源占用大，产品资源丰富且固件尺寸较大时，推荐使用|
|Quicklz|QBOOT_USING_QUICKLZ|768|4 (+1024) (+4096)|使用Quicklz解压缩功能|压缩率中，推荐使用|
|Fastlz|QBOOT_USING_FASTLZ|704|0 (+1024) (+4096)|使用Fastlz解压缩功能|压缩率略差，不推荐|
|OTA_download|QBOOT_USING_OTA_DOWNLOAD|2456|24|使用固件包下载功能，支持通过串口使用ymodem协议下载固件到flash，此功能依赖shell|资源足够时，推荐使用|
|shell|QBOOT_USING_SHELL|3268|8|使用shell命令行功能, 支持固件检测、克隆、恢复、校验等功能，此功能依赖shell|资源足够时，推荐使用|
|Product info|QBOOT_USING_PRODUCT_INFO|164|0|使用启动时输出产品信息，flash占用与用户显示信息多少有关|视需求使用|