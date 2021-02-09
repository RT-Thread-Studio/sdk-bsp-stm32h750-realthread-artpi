# Quick bootloader

## 1. 简介

**Quick bootloader** 是一个用于快速制作bootloader的组件。

### 1.1 目录结构

`Quick bootloader` 软件包目录结构如下所示：

``` 
qboot
├───doc                                // 说明文档目录
│   │   qboot各项配置资源占用情况说明.md  // 资源占用说明
│   │   qboot工作流程说明.md            // 工作流程说明
│   │   qboot命令详述.md            	  // shell命令说明
│   │   qboot使用指导.md               // 使用示例详解
│   │   极简版Bootloader制作.md        // 极简bootloader制作示例
│   └───qboot状态指示灯说明.md          // 状态指示灯说明
├───inc                               // 头文件目录
│   │   qboot.h                       // 主模块头文件
│   │   qboot_aes.h                   // aes解密模块头文件
│   │   qboot_fastlz.h                // fastlz解压模块头文件
│   │   qboot_gzip.h                  // gzip解压模块头文件
│   └───qboot_quicklz.h     	      // quicklz解压模块头文件
├───src                               // 源码目录
│   │   qboot.c                       // 主模块
│   │   qboot_aes.c                   // aes解密模块
│   │   qboot_fastlz.c                // fastlz解压模块
│   │   qboot_gzip.c                  // gzip解压模块
│   └───qboot_quicklz                 // quicklz解压模块
├───tools                             // 工具目录
│   └───QBootPackager_V1.00.zip       // 升级包打包器
│   license                           // 软件包许可证
│   readme.md                         // 软件包使用说明
└───SConscript                        // RT-Thread 默认的构建脚本
```

### 1.2 许可证

Quick bootloader 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT_Thread 4.0
- fal
- crclib

## 2. 使用

### 2.1 获取组件

- **方式1：**
通过 *Env配置工具* 或 *RT-Thread studio* 开启软件包，根据需要配置各项参数；配置路径为 *RT-Thread online packages -> system -> qboot* 

### 2.2 功能选项宏定义说明

| 选项宏 | 说明 |
| ---- | ---- |
| QBOOT_APP_PART_NAME 	    | 应用代码使用的fal分区名称
| QBOOT_DOWNLOAD_PART_NAME 	| 下载固件使用的fal分区名称
| QBOOT_FACTORY_PART_NAME 	| 出厂固件使用的fal分区名称
| QBOOT_USING_PRODUCT_CODE 	| 使用产品码验证，防止非法升级
| QBOOT_PRODUCT_CODE 	    | 定义产品码
| QBOOT_USING_AES 		    | 使用AES解密功能
| QBOOT_AES_IV 		    	| AES的16字节初始向量
| QBOOT_AES_KEY 		    | AES的32字节密钥
| QBOOT_USING_GZIP 			| 使用gzip解压缩功能
| QBOOT_USING_QUICKLZ 		| 使用quicklz解压缩功能
| QBOOT_USING_FASTLZ 		| 使用fastlz解压缩功能
| QBOOT_USING_SHELL 		| 使用命令行功能
| QBOOT_SHELL_KEY_CHK_TMO 	| 等待用户按键进入shell的超时时间
| QBOOT_USING_SYSWATCH 		| 使用系统看守组件
| QBOOT_USING_OTA_DOWNLOAD 	| 使用OTA downloader组件
| QBOOT_USING_PRODUCT_INFO 	| 使用启动时产品信息输出
| QBOOT_PRODUCT_NAME 	    | 产品名称
| QBOOT_PRODUCT_VER 	    | 产品版本
| QBOOT_PRODUCT_MCU 	    | 产品使用的MCU
| QBOOT_USING_STATUS_LED 	| 使用状态指示灯
| QBOOT_STATUS_LED_PIN 	    | 指示灯使用的引脚
| QBOOT_STATUS_LED_LEVEL 	| 指示灯的点亮电平
| QBOOT_USING_FACTORY_KEY 	| 使用恢复出厂按键
| QBOOT_FACTORY_KEY_PIN 	| 按键使用的引脚
| QBOOT_FACTORY_KEY_LEVEL 	| 按键按下后的引脚电平
| QBOOT_FACTORY_KEY_CHK_TMO | 检测按键持续按下的超时时间

### 2.3 各功能模块资源使用情况，详见 ：[qboot各项配置资源占用情况说明](https://gitee.com/qiyongzhong0/rt-thread-qboot/blob/master/doc/QBoot%E5%90%84%E9%A1%B9%E9%85%8D%E7%BD%AE%E8%B5%84%E6%BA%90%E5%8D%A0%E7%94%A8%E6%83%85%E5%86%B5%E8%AF%B4%E6%98%8E.md)

### 2.4 如何使用QBoot组件快速制作bootloader，详见：[QBoot使用指导](https://gitee.com/qiyongzhong0/rt-thread-qboot/blob/master/doc/QBoot%E4%BD%BF%E7%94%A8%E6%8C%87%E5%AF%BC.md)

## 3. 联系方式

* 维护：qiyongzhong
* 主页：https://gitee.com/qiyongzhong0/rt-thread-qboot
* 邮箱：917768104@qq.com

