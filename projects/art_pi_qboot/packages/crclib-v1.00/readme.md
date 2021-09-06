# CRC Library

## 1.简介

**Crc Library** 包括了常用8位、16位、32位CRC校验的函数库。

### 1.1目录结构

`crclib` 软件包目录结构如下所示：

``` 
crclib
├───inc                         // 头文件目录
│   │   crc.h                   // API 接口头文件
│   │   crc_cfg.h               // 配置头文件
│   │   typedef.h               // 数据类型定义头文件
│   │   crc8.h                  // 8位crc模块头文件
│   │   crc16.h                 // 16位crc模块头文件
│   └───crc32.h                 // 32位crc模块头文件
├───src                         // 源码目录
│   │   crc8.c                  // 8位crc模块
│   │   crc16.c                 // 16位crc模块
│   └───crc32.c                 // 32位crc模块
│   license                     // 软件包许可证
│   readme.md                   // 软件包使用说明
└───SConscript                  // RT-Thread 默认的构建脚本
```

### 1.2许可证

crclib package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3依赖

- 无

## 2.使用

### 2.1接口函数说明

####  void crc8_table_init(void);
- 功能 ：初始化8位crc校验表
- 参数 ：无
- 返回 ：无

####  u8 crc8_cyc_cal(u8 init_val, u8 *pdata, u32 len);
- 功能 ：循环计算8位crc校验值
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：8位crc计算结果

####  u8 crc8_cal(u8 *pdata, u32 len);
- 功能 ：计算8位crc校验值，初始值为0xFF，输出结果为计算值与0xFF的异或结果
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：8位crc计算结果

####  void crc16_table_init(void);
- 功能 ：初始化16位crc校验表
- 参数 ：无
- 返回 ：无

####  u16 crc16_cyc_cal(u16 init_val, u8 *pdata, u32 len);
- 功能 ：循环计算16位crc校验值
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：16位crc计算结果

####  u16 crc16_cal(u8 *pdata, u32 len);
- 功能 ：计算16位crc校验值，初始值为0xFFFF，输出结果为计算值与0xFFFF的异或结果
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：16位crc计算结果

####  void crc32_table_init(void);
- 功能 ：初始化32位crc校验表
- 参数 ：无
- 返回 ：无

####  u32 crc32_cyc_cal(u32 init_val, u8 *pdata, u32 len);
- 功能 ：循环计算32位crc校验值
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：32位crc计算结果

####  u32 crc32_cal(u8 *pdata, u32 len);
- 功能 ：计算32位crc校验值，初始值为0xFFFFFFFF，输出结果为计算值与0xFFFFFFFF的异或结果
- 参数 ：init_val--计算初始值
- 参数 ：pdata--数据缓冲区指针
- 参数 ：len--数据长度
- 返回 ：32位crc计算结果

### 2.2获取组件

- **方式1：**
通过 *Env配置工具* 或 *RT-Thread studio* 开启软件包，根据需要配置各项参数；配置路径为 *RT-Thread online packages -> miscellaneous packages -> crc library* 


### 2.3配置参数说明

| 参数宏 | 说明 |
| ---- | ---- |
| CRCLIB_USING_CRC8 		| 使用8位CRC
| CRCLIB_USING_CRC16 		| 使用16位CRC
| CRCLIB_USING_CRC32 		| 使用32位CRC
| CRC8_USING_CONST_TABLE 	| 使用8位CRC常量校验表
| CRC8_POLY 			    | 8位CRC校验多项式
| CRC8_INIT_VAL 			| 8位CRC校验初始值
| CRC16_USING_CONST_TABLE 	| 使用16位CRC常量校验表
| CRC16_POLY 			    | 16位CRC校验多项式
| CRC16_INIT_VAL 			| 16位CRC校验初始值
| CRC32_USING_CONST_TABLE 	| 使用32位CRC常量校验表
| CRC32_POLY 			    | 32位CRC校验多项式
| CRC32_INIT_VAL 			| 32位CRC校验初始值

## 3. 联系方式

* 维护：qiyongzhong
* 主页：https://gitee.com/qiyongzhong0/crclib


