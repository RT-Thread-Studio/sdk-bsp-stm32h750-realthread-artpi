# multi_rtimer

## 1 简介

multi_rtimer软件包是一个低功耗的实时软件定时器模块，理论上可以无限扩展应用所需的长\短定时器，可低功耗地管理时间触发任务。

multi_rtimer借鉴了LoRaMac timer 与 0x1abin/MultiTimer  
 - https://github.com/Lora-net/LoRaMac-node/blob/master/src/system/timer.c
 - https://github.com/0x1abin/MultiTimer

**主要特点：**

- 支持高精度低功耗定时
  - 定时精度最高可支持约31us（1/32768），缺省设置约1ms(1/1024)
- 支持长定时器
  - 比如min、hour等
- 支持在STOP模式下唤醒
  - 适用于超低功耗应用场景
 - 当前支持的MCU平台
  - STM32
    - STM32L0
    - STM32L4
    - STM32WL
    - STM32H7
        - 所需外设:RTC(支持Sub-Second Alarm)

## 2 使用说明

### 2.1 依赖

- 硬件RTC及其Alarm中断
  - 要求RTC支持Sub second

### 2.2 获取软件包

使用 multi_rtimer 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] multi_rtimer: a real-time and low power software timer module. --->
                Version (latest)  --->
                multi_rtimer options --->
                    [] multi_rtimer demo example
```

### 2.3 使用软件包

#### 2.3.1 multi_rtimer API

1. 硬件RTC初始化：
```
int hw_rtc_init(void);
```

该函数主要完成的功能有:

- RTC外设配置：
 - 设置RTC外设初始化、设置RTC date与time；
 - Alarm中断使能等；

2. 定义定时器管理对象
```
static timer_event_t led0_timer;
```

3. 初始化定时器对象，注册定时器回调函数
```
void rtimer_init( timer_event_t *obj, void ( *callback ))
```

4. 设置定时时间(ms)
```
void rtimer_set_value( timer_event_t *obj, uint32_t value )
```

5. 启动定时器
```
void rtimer_start( timer_event_t *obj )
```

6. 停止定时器
```
void rtimer_stop( timer_event_t *obj )
```

#### 2.3.2 LoRaWAN timer API

LoRaWAN timer API
```
#define TimerInit            rtimer_init
#define TimerStart           rtimer_start
#define TimerStop            rtimer_stop   
#define TimerReset           rtimer_reset
#define TimerSetValue        rtimer_set_value
#define TimerGetCurrentTime  rtimer_get_current_time

``` 

#### 2.3.3 应用层调用示例
```

/*!
 * \brief Function executed on Led 0 Timeout event
 */
static void on_led0_timeout(void)
{
    rtimer_stop( &led0_timer );
    
    rt_kprintf("on_led0_timeout,%d,on shot\r\n",rtimer_get_current_time());

}

/*!
 * \brief Function executed on Led 1 Timeout event
 */
static void on_led1_timeout(void)
{
    ////rtimer_stop( &led1_timer );
    rtimer_reset( &led1_timer );
    
    rt_kprintf("on_led1_timeout,%d\r\n",rtimer_get_current_time());
}

/*!
 * \brief Function executed on Led 3 Timeout event
 */
static void on_led3_timeout(void)
{
    interval_ms = interval_ms * 10;
    
    // stop timer then set new timer interval
    TimerSetValue( &led3_timer,interval_ms);
    TimerStart( &led3_timer );
    
    rt_kprintf("on_led3_timeout:%d,interval:%d ms\r\n",rtimer_get_current_time(),interval_ms);
}


int rt_multi_rtimer_init(void)
{
    hw_rtc_init();
    
    // on shot
    rtimer_init( &led0_timer, on_led0_timeout );
    rtimer_set_value( &led0_timer, 60000 ); // 60s
    
    rtimer_start( &led0_timer );
    rt_kprintf("led0_timer start,%d\r\n",rtimer_get_current_time());
    
    // periodicity
    rtimer_init( &led1_timer, on_led1_timeout );
    rtimer_set_value( &led1_timer, 1000 ); // 1000ms
    
    rtimer_start( &led1_timer );
    rt_kprintf("led1_timer start,%d\r\n",rtimer_get_current_time());
    
    // change the interval
    // Use LoRaWAN Timer API
    TimerInit( &led3_timer, on_led3_timeout );
    TimerSetValue( &led3_timer, interval_ms ); // 1ms * 10^n
    
    TimerStart( &led3_timer );
    rt_kprintf("led3_timer start,%d\r\n",rtimer_get_current_time());
    
    #ifdef RT_USING_PM
    rt_pm_request(PM_SLEEP_MODE_DEEP);
    #endif
    
    return 0;
}

```

## 3 联系人信息

维护人:

- [Forest-Rain](https://github.com/Forest-Rain) 
