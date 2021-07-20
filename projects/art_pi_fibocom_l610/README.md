# art-pi-fibocom-l610 例程

## 简介

本例程主要功能是让板载的 ADP-L610-ART-PI 板开机自动拨号。可执行阿里云、http api实现联网功能。


## 硬件说明
![uart](C:\Users\lenovo\Desktop\rt-thread\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_fibocom_l610\figures\uart.png)

ADP-L610-ART-PI 是基于广和通 CAT1模组(L610)。STM32于L610通过UART通信，使用AT指令实现拨号上网,L610模组内置lwip协议。通过蜂窝模组可以直连后台服务器。如：阿里云、腾讯云等。
该芯片硬件电路连接方式如上图所示。

## 软件说明

ADP-L610-ART-PI 的源代码位于 `/projects/art_pi_fibocom_l610/applications/main.c` 中。首先初始化L610 power io （PI5）,art-pi-fibocom-l610 默认上电开机，不用修改IO即可开机。若需要控制L610 powerio需根据实际控制口做修改。

```
#define LED_PIN GET_PIN(I, 8)
```

在 main 函数中，初始化L610power IO。

```
#define LED_PIN GET_PIN(I, 8)

void l610_power_init(void)
{
    rt_pin_mode(GET_PIN(I, 5), PIN_MODE_OUTPUT);
}

int main(void)
{
    rt_uint32_t count = 1;

    l610_power_init();
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

ping baidu.com

![ping_baidu](C:\Users\lenovo\Desktop\rt-thread\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_fibocom_l610\figures\ping_baidu.png)

运行 web_get_test指令：

![http_get](C:\Users\lenovo\Desktop\rt-thread\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_fibocom_l610\figures\http_get.png)

运行 ali_mqtt_sample

![connect_aliyun](C:\Users\lenovo\Desktop\rt-thread\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_fibocom_l610\figures\connect_aliyun.png)

## 注意事项

1、主要L610模块uart电压为1.8V和单片机（一般为3.3v）通信时注意增加电压转换。

2、ADP-L610-ART-PI 已经添加UART电压转换电路。如下图：

![uart_voltage_conversion](C:\Users\lenovo\Desktop\rt-thread\sdk-bsp-stm32h750-realthread-artpi\projects\art_pi_fibocom_l610\figures\uart_voltage_conversion.png)