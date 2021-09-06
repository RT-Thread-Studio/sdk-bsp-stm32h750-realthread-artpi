/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-09-04     armink       the first version
 */

#include <rthw.h>
#include <ulog.h>

#ifdef ULOG_BACKEND_USING_CONSOLE

#if defined(ULOG_ASYNC_OUTPUT_BY_THREAD) && ULOG_ASYNC_OUTPUT_THREAD_STACK < 384
#error "The thread stack size must more than 384 when using async output by thread (ULOG_ASYNC_OUTPUT_BY_THREAD)"
#endif

static struct ulog_backend console;

void ulog_console_backend_output(struct ulog_backend *backend, rt_uint32_t level, const char *tag, rt_bool_t is_raw,
        const char *log, size_t len)
{
#ifdef RT_USING_DEVICE
    rt_device_t dev = rt_console_get_device();

    if (dev == RT_NULL)
    {
        rt_hw_console_output(log);
    }
    else
    {
        rt_uint16_t old_flag = dev->open_flag;

        dev->open_flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(dev, 0, log, len);
        dev->open_flag = old_flag;
    }
#else
    rt_hw_console_output(log);
#endif

}

int ulog_console_backend_init(void)
{
    ulog_init();
    console.output = ulog_console_backend_output;

    ulog_backend_register(&console, "console", RT_TRUE);

    return 0;
}
INIT_PREV_EXPORT(ulog_console_backend_init);

#endif /* ULOG_BACKEND_USING_CONSOLE */
