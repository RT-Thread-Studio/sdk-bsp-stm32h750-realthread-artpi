# GT9147

## 简介

gt9147 软件包提供了使用触摸芯片 gt9147 基本功能，并且本软件包已经对接到了 Touch 框架，通过 Touch 框架，开发者可以快速的将此触摸芯片驱动起来。
## 支持情况

| 包含设备           | 触摸芯片 |  
| ----------------     | -------- | 
| **通讯接口**      |          |      
| IIC              | √        | 
| **工作模式**     |          |     
| 中断             | √        | 
| 轮询             |   √       |        

## 使用说明

### 依赖

- RT-Thread 4.0.0+
- Touch 组件
- I2C 驱动：gt9147 设备使用 I2C 进行数据通讯，需要系统 I2C 驱动支持；

### 获取软件包

使用 gt9147 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
  peripheral libraries and drivers  --->
    touch drivers  --->
      gt9147: touch ic gt9147 for rt-thread
              Version (latest)  --->
```
**Version**：软件包版本选择

### 使用软件包

gt9147 软件包初始化函数如下所示：

```
int rt_hw_gt9147_init(const char *name, struct rt_touch_config *cfg)
```

该函数需要由用户调用，函数主要完成的功能有，

- 设备配置和初始化（根据传入的配置信息，配置接口设备和中断引脚）；
- 注册相应的传感器设备，完成 gt9147 设备的注册；

#### 初始化示例

```.c
int rt_hw_gt9147_port(void)
{
    struct rt_touch_config config;
    rt_uint8_t rst;
    
    rst = GT9147_RST_PIN;
    config.dev_name = "i2c1";
    config.irq_pin.pin  = GT9147_IRQ_PIN;
    config.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    config.user_data = &rst;

    rt_hw_gt9147_init("gt", &config);

    return 0;
}
INIT_ENV_EXPORT(rt_hw_gt9147_port);
```

## 注意事项

暂无

## 联系人信息

维护人:

- [tyustli](https://github.com/tyustli) 

- 主页：<https://github.com/RT-Thread-packages/gt9147>