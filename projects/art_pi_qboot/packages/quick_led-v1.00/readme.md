# Quick Led

## 1.简介

**Quick Led** 旨在提供一个快捷易用的led驱动包。

### 1.1目录结构

`Quick Led` 软件包目录结构如下所示：

``` 
qled
├───inc   						// 头文件目录
│   |   qled.h            		// API 接口头文件
│   └───qled_sample.h     		// 示例头文件
├───src                   		// 源码目录
│   |   qled.c            		// 主模块
│   └───qled_samplec  			// 示例模块
│	license  					// 软件包许可证
│	readme.md					// 软件包使用说明
└───SConscript					// RT-Thread 默认的构建脚本
```

### 1.2许可证

Quick Led package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3依赖

- RT_Thread 4.0

## 2.使用

### 2.1接口函数说明

#### int qled_add(int pin, int level);
- 功能 ：加一个led到驱动
- 参数 ：pin--led使用的引脚号
- 参数 ：level--点亮led的电平
- 返回 ：0--成功，其它--错误

#### void qled_remove(int pin);
- 功能 ：从驱动移除led
- 参数 ：pin--led使用的引脚号
- 返回 ：无

#### int qled_set_off(int pin);
- 功能 ：设置led到常灭状态
- 参数 ：pin--led使用的引脚号
- 返回 ：0--成功，其它--错误

#### int qled_set_on(int pin);
- 功能 ：设置led到常亮状态
- 参数 ：pin--led使用的引脚号
- 返回 ：0--成功，其它--错误

#### int qled_set_blink(int pin, int ton_ms, int toff_ms);
- 功能 ：设置led到周期性闪烁状态
- 参数 ：pin--led使用的引脚号
- 参数 ：ton_ms--led点亮时长
- 参数 ：toff_ms--led熄灭时长
- 返回 ：0--成功，其它--错误

#### int qled_set_special(int pin, const u16 *datas, int data_total, void (*over_cb)(void));
- 功能 ：设置led执行特殊序列闪烁
- 参数 ：pin--led使用的引脚号
- 参数 ：datas--特殊序列数据
- 参数 ：data_total--特殊序列的数据总数
- 参数 ：over_cb--特殊序列执行结束时的回调函数
- 返回 ：0--成功，其它--错误

### 2.2使用示例

#### 示例1. 以指定频率和占空比控制led闪烁

```
qled_add(24, 1);//加24号引脚led到驱动，高电平点亮

qled_set_blink(24, 50, 50);//设置led以10Hz闪烁，亮50ms，灭50ms
rt_thread_mdelay(5000);//时间5s

qled_set_blink(24, 50, 450);//设置led以2Hz闪烁，亮50ms，灭450ms
rt_thread_mdelay(10000);//时间10s

qled_remove(24);//不再需要led了，从驱动中移除
```

#### 示例2. 发送SOS信号

```
#define QLED_SOS_PIN GET_PIN(B, 9) //25号

static int sos_send_times = 0;//发送sos信号次数计数
static const u16 sos_datas[] = //定义sos信号时间数据
{
    200, 200, 200, 200, 200, 200,       //short 3 times
    600, 600, 600, 600, 600, 600,       //long 3 times
    200, 200, 200, 200, 200, 200 + 2000 //short 3 times and 2000ms interval
};

static void qled_sos_cb(void)//定义特殊序列执行结束回调函数
{
    sos_send_times--;
    if (sos_send_times > 0)//执行次数未到
    {
        qled_set_special(QLED_SOS_PIN, sos_datas, sizeof(sos_datas)/sizeof(u16), qled_sos_cb);//再次执行
    }
    else//执行完成
    {
        qled_remove(QLED_SOS_PIN);//不需要了，从驱动移除
    }
}

void qled_send_sos(void)//执行发送sos信号，发送5次SOS信号, 总用时40s
{
    sos_send_times = 5;//设置发送次数为5
    qled_add(QLED_SOS_PIN, 1);//加引脚led到驱动
    qled_set_special(QLED_SOS_PIN, sos_datas, sizeof(sos_datas)/sizeof(u16), qled_sos_cb);//启动执行特殊序列
}
```

### 2.3获取组件

- **方式1：**
通过 *Env配置工具* 或 *RT-Thread studio* 开启软件包，根据需要配置各项参数；配置路径为 *RT-Thread online packages -> peripherals packages -> quick led* 


### 2.4配置参数说明

| 参数宏 | 说明 |
| ---- | ---- |
| QLED_TOTAL 				| 支持的led总数
| QLED_TIME_UNIT_MS 		| led闪烁的时间单位
| QLED_THREAD_NAME 			| led驱动线程名称
| QLED_THREAD_STACK_SIZE 	| led驱动线程堆栈尺寸
| QLED_THREAD_PRIO 			| led驱动线程优先级
| QLED_USING_SAMPLE 		| 使用示例
| QLED_RUN_PIN 				| 示例运行灯引脚 
| QLED_SOS_PIN 				| 示例SOS信号灯引脚

## 3. 联系方式

* 维护：qiyongzhong
* 主页：https://gitee.com/qiyongzhong0/rt-thread-qled
* 邮箱：917768104@qq.com
