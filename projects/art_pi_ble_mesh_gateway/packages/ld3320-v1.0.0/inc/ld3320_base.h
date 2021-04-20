/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-08     xqyjlj       the first version
 */
#ifndef __LD3320_BASE_H__
#define __LD3320_BASE_H__
#include <stdint.h>
#include <rtdevice.h>

void ld3320_write_reg(struct rt_spi_device *device, uint8_t addr, uint8_t data);
uint8_t ld3320_read_reg(struct rt_spi_device *device, uint8_t addr);


#endif /* __LD3320_BASE_H__ */
