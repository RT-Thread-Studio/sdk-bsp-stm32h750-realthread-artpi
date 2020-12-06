/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-04-23
 */

#ifndef _SFUD_CFG_H_
#define _SFUD_CFG_H_

#include <rtconfig.h>

/**
 * It will print more information on debug mode.
 * #define RT_DEBUG_SFUD open debug mode */
#ifdef RT_DEBUG_SFUD
#define SFUD_DEBUG_MODE
#endif

#ifdef RT_DEBUG_SFUD
#define DBG_LVL DBG_LOG
#define SFUD_DEBUG(fmt, ...)  LOG_D("(%s:%ld) "fmt"", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_LVL DBG_INFO
#endif /* RT_DEBUG_SFUD */

#define DBG_TAG "SFUD"
#include <rtdbg.h>
extern void rt_kprintf(const char *fmt, ...);
#define SFUD_INFO(...)        LOG_I(__VA_ARGS__)

/**
 * Using probe flash JEDEC SFDP parameter.
 */
#ifdef RT_SFUD_USING_SFDP
#define SFUD_USING_SFDP
#endif

/**
 * SFUD will support QSPI mode.
 */
#ifdef RT_SFUD_USING_QSPI
#define SFUD_USING_QSPI
#endif

/**
 * Using probe flash JEDEC ID then query defined supported flash chip information table. @see SFUD_FLASH_CHIP_TABLE
 */
#ifdef RT_SFUD_USING_FLASH_INFO_TABLE
#define SFUD_USING_FLASH_INFO_TABLE
#endif

#define SFUD_FLASH_DEVICE_TABLE {{0}}

#endif /* _SFUD_CFG_H_ */
