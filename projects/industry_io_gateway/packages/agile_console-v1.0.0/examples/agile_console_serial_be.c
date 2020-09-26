#include <rtthread.h>

#ifdef PKG_USING_AGILE_CONSOLE
#ifdef PKG_USING_AGILE_CONSOLE_SERIAL_EXAMPLE

#include <agile_console.h>

static rt_device_t serial_dev = RT_NULL;

static struct agile_console_backend serial_backend = {0};

static void serial_backend_output(const uint8_t *buf, int len)
{
    if(serial_dev == RT_NULL)
        return;
    
    rt_device_write(serial_dev, 0, buf, len);
}

static int serial_backend_read(uint8_t *buf, int len)
{
    if(serial_dev == RT_NULL)
        return 0;
    
    return rt_device_read(serial_dev, 0, buf, len);
}

static int agile_colsole_serial_board_init(void)
{
    serial_dev = rt_device_find(PKG_AGILE_CONSOLE_SERIAL_DEVICE_NAME);
    if(serial_dev == RT_NULL)
        return -RT_ERROR;
    
    if(rt_device_open(serial_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM) != RT_EOK)
        return -RT_ERROR;
    
    serial_backend.output = serial_backend_output;
    serial_backend.read = serial_backend_read;

    agile_console_backend_register(&serial_backend);

    return RT_EOK;
}
INIT_BOARD_EXPORT(agile_colsole_serial_board_init);

static int agile_console_serial_input_init(void)
{
    if(serial_dev == RT_NULL)
        return -RT_ERROR;
    
    int ret = rt_device_open(serial_dev, RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_RDWR);

    return ret;
}
INIT_DEVICE_EXPORT(agile_console_serial_input_init);

#endif /* PKG_USING_AGILE_CONSOLE_SERIAL_EXAMPLE */
#endif /* PKG_USING_AGILE_CONSOLE */


