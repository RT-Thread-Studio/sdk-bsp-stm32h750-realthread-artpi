# 持续开发记录

## 简介

此项目基于例程持续开发（art_pi_blink_led），目的用于开发驱动与学习STM32系列单片机嵌入式软件相关知识。

## 功能

### 调试

基于rt-thread	DEBUG工具调试。

```
如：
#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>
LOG_E(...)
LOG_W(...)
LOG_I(...)
LOG_L(...)

/* DEBUG level */
#define DBG_ERROR           0
#define DBG_WARNING         1
#define DBG_INFO            2
#define DBG_LOG             3
```



## 硬件说明
<img src="./figures/blink_pcb.png" alt="LED 连接单片机引脚" style="zoom: 50%;" />
如上图所示，RGB-LED 属于共阳 LED， **阴极** 分别与单片机的引脚相连，其中蓝色 LED 对应 PI8 引脚。单片机引脚输出低电平即可点亮 LED，输出高电平则会熄灭 LED。

## 软件说明

闪灯的源代码位于 `/projects/art_pi_blink_led/applications/main.c` 中。首先定义了一个宏 `LED_PIN` ，代表闪灯的 LED 引脚编号，然后与 `GPIO_LED_B`（**PI8**）对应：

```
#define LED_PIN GET_PIN(I, 8)
```

在 main 函数中，将该引脚配置为输出模式，并在下面的 while 循环中，周期性（500毫秒）开关 LED。

```
int main(void)
{
    rt_uint32_t count = 1;

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while(count++)
    {
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
    }
    return RT_EOK;
}
```



## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

正常运行后，蓝色 LED 会周期性闪烁。

## 注意事项

如果想要修改`LED_PIN` 宏定义，可以通过 GET_PIN 来修改。

