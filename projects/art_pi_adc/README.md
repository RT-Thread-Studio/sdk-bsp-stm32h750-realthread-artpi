# ADC测试例程

## 简介

本例程主要功能是使用RT-Thread（以下简称RTT）测试ART_PI（stm32h750）内部ADC的性能。本例程基于RT-Thread Studio（以下简称RTTStudio）开发测试，未在MDK环境下编译运行。在文档中还包括有如何在基于Art_Pi BSP创建的工程中增加ADC功能，可以作为启用其他外设功能的参考方法。

## 硬件说明
![image-20201202121836548](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202121836548.png)
如上图所示，例程中使用STM32H750的PA6作为ADC检测口。PA6在Art_Pi 开发板P2排座的第23脚。测试时可以方便的用跳线帽或者排线将PA6和GND连接测试。

## 配置说明

在使用ADC之前，需要经过以下步骤：

### 1、在HAL层初始化PA6为内部ADC通道

​       配置PA6为ADC需使用ST公司的STM32CubeMX，先使用STM32CubeMX下载安装STM32H7的官方HAL包，然后打开RTTStudio生成的BSP工程代码的board文件夹，找到CubeMX_Config.ioc文件（请以具体工程目录为准，参考目录...\\...\workspace\art_pi_adc\board\CubeMX_Config.ioc）,选中PA6后选择为ADC，参考配置参数如下图：

![image-20201202123824886](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202123824886.png)

保存配置，关闭软件。至此，已完成在HAL层配置底层硬件的目标。

### 2、在Kconfig中添加ADC配置项

​	配置完HAL后，需要在Kconfig文件增加配置项，用于在menuconfig时可以选择和修改对应的引脚。Kconfig文件在工程目录的libraries文件夹下，使用VSCode或其他编辑器可以编辑或直接使用RTTStudio也很方便。由于ADC属于片上资源，所以建议在“menu "On-chip Peripheral"”下面添加内容，具体增加内容如下图：

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202124612870.png" alt="image-20201202124612870" style="zoom:67%;" />

**注意**：此处编写时语法很简单，但一定要注意资源命名与依赖项的一致，本人就在此处因为写错了导致工程无法添加drv_adc.c文件。

### 3、在SConscript中添加依赖

在配置项中添加完后，还需要在SConscript中添加依赖，否则编译时无法将drv_adc文件添加到工程中，虽编译不会报错，但是无法list到设备，也无法使用ADC。

SConscript在工程目录的libraries文件夹的drivers文件夹下，可以使用VSCode或RTTStudio软件打开编辑，主要就是添加对工程对ADC的依赖，具体添加内容如下：

![image-20201202125536758](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202125536758.png)

**注意**：此处编写时语法很简单，但一定要注意命名与配置项的一致，本人就在此处因为写错了导致工程无法添加drv_adc.c文件。

### 4、启用RTT的adc驱动和选择对应引脚

以上文件修改完成后，需要在工程中选择使用adc驱动和配置引脚。使用RTTStudio打开工程，双击“RT-Thread Settings”，点击右侧小箭头切换到配置页面，在“组件”子页面下选择“设备驱动程序”，勾选“使用ADC设备驱动程序”，选中后效果如下图所示：

![image-20201202130133429](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202130133429.png)

再在“硬件”子选项下打开“On-chip Peripheral”，点击“Enable ADC1”，输入对应的GPIO编号，本例程此处选择的PA6，引脚编号请查看drv_gpio.c。按ctrl+s保存配置，此时RTTStudio会将配置信息保存在工程中。

![image-20201202130604602](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20201202130604602.png)

至此，配置工作基本完成，下来就是在源代码中编辑内容，实现adc的应用了。

## 软件说明

源代码位于 `/projects/art_pi_adc/applications/main.c` 中。首先定义了几个宏 ，然后编写一个adc_vol函数实现adc的打开、读数、计算和输出（参照文档中心的adc部分内容），最后用MSH_CMD_EXPORT实现了msh读取adc值的应用。

```
#define LED_PIN GET_PIN(I, 8)
#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL     3           /* ADC 通道 */
#define REFER_VOLTAGE       330         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 16)   /* 转换位数为16位 */

static int adc_vol(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* 读取采样值 */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
    rt_kprintf("the value is :%d \n", value);

    /* 转换为对应电压值 */
    vol = value * REFER_VOLTAGE / CONVERT_BITS;
    rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}
```

MSH_CMD_EXPORT(adc_vol, adc voltage convert sample);

## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板，打开超级终端，连接串口。

### 运行效果

正常运行后，输入help，可以看到最后一行一出现adc_vol，然后输入adc_vol即可读取到数据。

## 注意事项

配置项和依赖项的命名一定要一致，此外，在stm32cubemx中配置底层时，尽量少改动其他配置。

