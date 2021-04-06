/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-14     Ghazigq     the first version
 */

#ifndef __GBK2UTF8_H__
#define __GBK2UTF8_H__

#include <rtthread.h>

#ifdef PKG_USING_GBK2UTF8

    /*数组大小*/
    #undef  ARRAY_SIZE
    #define ARRAY_SIZE(a)       ((sizeof(a))/(sizeof((a)[0])))

    /***********************************************************
    * 函数名称：gbkk2utf8
    * 功能描述：将 gbk转为 utf8
    * 输入参数：pin_buf 输入缓冲区
    *       in_len  输入长度
    * 输出参数：ptr 成功后的起始位置(malloc产生),(数据末尾加上'0')
    * 返 回 值：  转换后的长度, < 0 失败,其它成功
    ***********************************************************/
    int gbk2utf8(char **ptr, void *pin_buf, int in_len);

    /***********************************************************
    * 函数名称：gbkk2utf8
    * 功能描述：将 gbk转为 utf8
    * 输入参数：pin_buf 输入缓冲区
    *       in_len  输入长度
    * 输出参数：ptr 成功后的起始位置(malloc产生),(数据末尾加上'0')
    * 返 回 值：  转换后的长度, < 0 失败,其它成功
    ***********************************************************/
    int utf82gbk(char **ptr, void *pin_buf, int in_len);

#endif /* PKG_USING_GBK2UTF8 */

#endif /* __GBK2UTF8_H__ */

