/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"


#define LED_PIN GET_PIN(I, 8)
#define ADC_DEV_NAME        "adc1"      /* ADC �豸���� */
#define ADC_DEV_CHANNEL     3           /* ADC ͨ�� */
#define REFER_VOLTAGE       330         /* �ο���ѹ 3.3V,���ݾ��ȳ���100����2λС��*/
#define CONVERT_BITS        (1 << 16)   /* ת��λ��Ϊ12λ */

static int adc_vol(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* �����豸 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* ʹ���豸 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* ��ȡ����ֵ */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
    rt_kprintf("the value is :%d \n", value);

    /* ת��Ϊ��Ӧ��ѹֵ */
    vol = value * REFER_VOLTAGE / CONVERT_BITS;
    rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

    /* �ر�ͨ�� */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}

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

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);
MSH_CMD_EXPORT(adc_vol, adc voltage convert sample);

