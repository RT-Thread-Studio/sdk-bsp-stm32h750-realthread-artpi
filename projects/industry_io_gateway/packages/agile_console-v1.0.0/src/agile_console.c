/*************************************************
 All rights reserved.
 File name:     agile_console.c
 Description:   agile_console源码
 History:
 1. Version:      v1.0.0
    Date:         2020-08-01
    Author:       Longwei Ma
    Modification: 新建版本
*************************************************/

#include <rthw.h>

#ifdef PKG_USING_AGILE_CONSOLE

#include <agile_console.h>

#ifndef PKG_AGILE_CONSOLE_RX_BUFFER_SIZE
#define PKG_AGILE_CONSOLE_RX_BUFFER_SIZE        256
#endif

#ifndef PKG_AGILE_CONSOLE_DEVICE_NAME
#define PKG_AGILE_CONSOLE_DEVICE_NAME           "acon"
#endif

#ifndef PKG_AGILE_CONSOLE_THREAD_PRIORITY
#define PKG_AGILE_CONSOLE_THREAD_PRIORITY       19
#endif

#ifndef PKG_AGILE_CONSOLE_THREAD_STACK_SIZE
#define PKG_AGILE_CONSOLE_THREAD_STACK_SIZE     2048
#endif

static uint8_t agile_console_rb_buffer[PKG_AGILE_CONSOLE_RX_BUFFER_SIZE];
static rt_slist_t backend_slist = RT_SLIST_OBJECT_INIT(backend_slist);
static struct agile_console console = {0};


#ifdef RT_USING_POSIX
#include <dfs_posix.h>
#include <dfs_poll.h>

#ifdef RT_USING_POSIX_TERMIOS
#include <posix_termios.h>
#endif

static rt_err_t console_fops_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_wqueue_wakeup(&(dev->wait_queue), (void*)POLLIN);

    return RT_EOK;
}

/* fops for console */
static int console_fops_open(struct dfs_fd *fd)
{
    rt_err_t ret = 0;
    rt_uint16_t flags = 0;
    rt_device_t device;

    device = (rt_device_t)fd->data;
    RT_ASSERT(device != RT_NULL);

    flags = RT_DEVICE_FLAG_RDWR;

    if ((fd->flags & O_ACCMODE) != O_WRONLY)
    {
        rt_base_t level = rt_hw_interrupt_disable();
        rt_device_set_rx_indicate(device, console_fops_rx_ind);
        rt_hw_interrupt_enable(level);
    }
        
    ret = rt_device_open(device, flags);
    if (ret == RT_EOK) return 0;

    return ret;
}

static int console_fops_close(struct dfs_fd *fd)
{
    rt_device_t device;

    device = (rt_device_t)fd->data;

    rt_base_t level = rt_hw_interrupt_disable();
    rt_device_set_rx_indicate(device, RT_NULL);
    rt_hw_interrupt_enable(level);

    rt_device_close(device);

    return 0;
}

static int console_fops_ioctl(struct dfs_fd *fd, int cmd, void *args)
{
    rt_device_t device;

    device = (rt_device_t)fd->data;
    switch (cmd)
    {
    case FIONREAD:
        break;
    case FIONWRITE:
        break;
    }

    return rt_device_control(device, cmd, args);
}

static int console_fops_read(struct dfs_fd *fd, void *buf, size_t count)
{
    int size = 0;
    rt_device_t device;

    device = (rt_device_t)fd->data;

    do
    {
        size = rt_device_read(device, -1,  buf, count);
        if (size <= 0)
        {
            if (fd->flags & O_NONBLOCK)
            {
                size = -EAGAIN;
                break;
            }

            rt_wqueue_wait(&(device->wait_queue), 0, RT_WAITING_FOREVER);
        }
    }while (size <= 0);

    return size;
}

static int console_fops_write(struct dfs_fd *fd, const void *buf, size_t count)
{
    rt_device_t device;

    device = (rt_device_t)fd->data;
    return rt_device_write(device, -1, buf, count);
}

static int console_fops_poll(struct dfs_fd *fd, struct rt_pollreq *req)
{
    int mask = 0;
    int flags = 0;
    rt_device_t device;

    device = (rt_device_t)fd->data;
    RT_ASSERT(device != RT_NULL);

    /* only support POLLIN */
    flags = fd->flags & O_ACCMODE;
    if (flags == O_RDONLY || flags == O_RDWR)
    {
        rt_base_t level;

        rt_poll_add(&(device->wait_queue), req);

        level = rt_hw_interrupt_disable();
        if(rt_ringbuffer_data_len(&(console.rx_rb)) > 0)
            mask |= POLLIN;
        rt_hw_interrupt_enable(level);
    }

    return mask;
}

const static struct dfs_file_ops _console_fops =
{
    console_fops_open,
    console_fops_close,
    console_fops_ioctl,
    console_fops_read,
    console_fops_write,
    RT_NULL, /* flush */
    RT_NULL, /* lseek */
    RT_NULL, /* getdents */
    console_fops_poll,
};
#endif


static rt_size_t agile_console_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    if(size == 0)
        return 0;

    rt_size_t result = 0;

    rt_base_t level = rt_hw_interrupt_disable();
    result = rt_ringbuffer_get(&(console.rx_rb), buffer, size);
    rt_hw_interrupt_enable(level);

    return result;
}

static rt_size_t agile_console_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    if(size == 0)
        return 0;

    rt_slist_t *node;
    rt_slist_for_each(node, &backend_slist)
    {
        struct agile_console_backend *backend = rt_slist_entry(node, struct agile_console_backend, slist);
        if(backend->output == RT_NULL)
            continue;
        
        backend->output(buffer, size);
    }

    return size;
}

static rt_err_t agile_console_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    switch(cmd)
    {
#ifdef RT_USING_POSIX_TERMIOS
        case TCFLSH:
        {
            rt_base_t level = rt_hw_interrupt_disable();
            rt_ringbuffer_reset(&(console.rx_rb));
            rt_hw_interrupt_enable(level);
        }
        break;
#endif

        default:
            result = -RT_ERROR;
        break;
    }

    return result;
}


#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops _console_ops = 
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    agile_console_read,
    agile_console_write,
    agile_console_control
};
#endif

static int agile_console_board_init(void)
{
    rt_memset(&console, 0, sizeof(struct agile_console));

    rt_memset(agile_console_rb_buffer, 0, PKG_AGILE_CONSOLE_RX_BUFFER_SIZE);
    rt_ringbuffer_init(&(console.rx_rb), agile_console_rb_buffer, PKG_AGILE_CONSOLE_RX_BUFFER_SIZE);

    struct rt_device *device = &(console.parent);
    
    device->type        = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &_console_ops;
#else
    device->init        = RT_NULL;
    device->open        = RT_NULL;
    device->close       = RT_NULL;
    device->read        = agile_console_read;
    device->write       = agile_console_write;
    device->control     = agile_console_control;
#endif
    device->user_data   = RT_NULL;

    /* register a character device */
    rt_device_register(device, PKG_AGILE_CONSOLE_DEVICE_NAME, RT_DEVICE_FLAG_RDWR);

#if defined(RT_USING_POSIX)
    /* set fops */
    device->fops        = &_console_fops;
#endif

    rt_console_set_device(PKG_AGILE_CONSOLE_DEVICE_NAME);

    return RT_EOK;
}
INIT_BOARD_EXPORT(agile_console_board_init);


static void agile_console_input_entry(void* parameter)
{
    uint8_t read_buf[100];
    int read_len = 0;
    rt_slist_t *node;

    while(1)
    {
        rt_thread_mdelay(10);

        rt_slist_for_each(node, &backend_slist)
        {
            struct agile_console_backend *backend = rt_slist_entry(node, struct agile_console_backend, slist);
            if(backend->read == RT_NULL)
                continue;
            
            do
            {
                read_len = backend->read(read_buf, sizeof(read_buf));
                if(read_len > 0)
                {
                    rt_enter_critical();

                    rt_base_t level = rt_hw_interrupt_disable();
                    rt_ringbuffer_put(&(console.rx_rb), read_buf, read_len);
                    rt_hw_interrupt_enable(level);

                    if(console.parent.rx_indicate != RT_NULL)
                        console.parent.rx_indicate(&(console.parent), read_len);
                    
                    rt_exit_critical();
                }
            }while(read_len > 0);
        }
    }
}

static int agile_console_input_init(void)
{
    rt_thread_t tid = rt_thread_create("acon", agile_console_input_entry, RT_NULL, PKG_AGILE_CONSOLE_THREAD_STACK_SIZE, 
                                       PKG_AGILE_CONSOLE_THREAD_PRIORITY, 100);
    
    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    return RT_EOK;
}
INIT_ENV_EXPORT(agile_console_input_init);

int agile_console_backend_register(struct agile_console_backend *backend)
{
    rt_base_t level;

    rt_slist_init(&(backend->slist));

    level = rt_hw_interrupt_disable();

    rt_slist_append(&backend_slist, &(backend->slist));

    rt_hw_interrupt_enable(level);

    return RT_EOK;
}

#endif /* PKG_USING_AGILE_CONSOLE */
