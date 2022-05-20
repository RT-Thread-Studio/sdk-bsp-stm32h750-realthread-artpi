#include <rtthread.h>

#ifdef PKG_USING_AGILE_CONSOLE
#ifdef PKG_AGILE_CONSOLE_USING_TINYUSB

#include <stdint.h>
#include <rtdevice.h>
#include <agile_console.h>
#include <tusb.h>

ALIGN(RT_ALIGN_SIZE)
static struct rt_semaphore _sem;
static struct rt_timer _timer;
static struct rt_ringbuffer _rb;
static uint8_t _rb_buf[1024];

static struct rt_thread _thread;
static uint8_t _thread_stack[512];

static struct agile_console_backend _console_backend = {0};
static uint8_t _shield_flag = 0;

static void console_backend_output(rt_device_t dev, const uint8_t *buf, int len)
{
    if (_shield_flag)
        return;

    if (!tud_cdc_connected())
        return;

    rt_base_t level = rt_hw_interrupt_disable();

    if (dev->open_flag & RT_DEVICE_FLAG_STREAM) {
        while (len > 0) {
            if (*buf == '\n') {
                if (rt_ringbuffer_putchar(&_rb, '\r') == 0)
                    break;
            }

            if (rt_ringbuffer_putchar(&_rb, *buf) == 0)
                break;

            ++buf;
            --len;
        }
    } else
        rt_ringbuffer_put(&_rb, buf, len);

    rt_hw_interrupt_enable(level);

    rt_sem_release(&_sem);
}

static int console_backend_read(rt_device_t dev, uint8_t *buf, int len)
{
    if (_shield_flag)
        return 0;

    if (!tud_cdc_connected())
        return 0;

    if (!tud_cdc_available())
        return 0;

    return tud_cdc_read(buf, len);
}

static void timer_timeout(void *parameter)
{
    if (_shield_flag)
        return;

    if (tud_cdc_available())
        agile_console_wakeup();
}

static void con_usb_output_entry(void *parameter)
{
    rt_base_t level;
    rt_uint8_t *send_ptr = RT_NULL;
    int send_len;

    while (1) {
        do {
            level = rt_hw_interrupt_disable();
            send_len = rt_ringbuffer_peak(&_rb, &send_ptr);
            rt_hw_interrupt_enable(level);

            if (send_len > 0) {
                if (_shield_flag)
                    continue;

                if (!tud_cdc_connected())
                    continue;

                tud_cdc_write(send_ptr, send_len);
                tud_cdc_write_flush();
            }
        } while (send_len > 0);

        rt_sem_take(&_sem, RT_WAITING_FOREVER);
        rt_sem_control(&_sem, RT_IPC_CMD_RESET, RT_NULL);
    }
}

static int tinyusb_console_init(void)
{
    rt_sem_init(&_sem, "con_usb", 0, RT_IPC_FLAG_FIFO);
    rt_timer_init(&_timer, "con_usb", timer_timeout, RT_NULL, 10, RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&_timer);
    rt_ringbuffer_init(&_rb, _rb_buf, sizeof(_rb_buf));

    _console_backend.output = console_backend_output;
    _console_backend.read = console_backend_read;
    agile_console_backend_register(&_console_backend);

    rt_thread_init(&_thread,
                   "con_usb",
                   con_usb_output_entry,
                   RT_NULL,
                   &_thread_stack[0],
                   sizeof(_thread_stack),
                   RT_THREAD_PRIORITY_MAX - 6,
                   100);

    rt_thread_startup(&_thread);

    return RT_EOK;
}
INIT_ENV_EXPORT(tinyusb_console_init);

static int tinyusb_console_disable(void)
{
    _shield_flag = 1;

    return RT_EOK;
}
MSH_CMD_EXPORT(tinyusb_console_disable, disable tinyusb console);

static int tinyusb_console_enable(void)
{
    _shield_flag = 0;
    tud_cdc_read_flush();

    return RT_EOK;
}
MSH_CMD_EXPORT(tinyusb_console_enable, enable tinyusb console);

#endif /* PKG_AGILE_CONSOLE_USING_TINYUSB */
#endif /* PKG_USING_AGILE_CONSOLE */
