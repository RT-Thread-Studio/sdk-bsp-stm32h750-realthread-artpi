#include "littlevgl2rtt.h"
#include "lvgl.h"

static rt_device_t device;
static struct rt_device_graphic_info info;
static struct rt_messagequeue *input_mq;
static int _lv_init = 0;
static lv_disp_drv_t disp_drv;
static lv_disp_buf_t disp_buf;

static void color_to16_maybe(lv_color16_t *dst, lv_color_t *src)
{
#if (LV_COLOR_DEPTH == 16)
    dst->full = src->full;
#else
    dst->ch.blue = src->ch.blue;
    dst->ch.green = src->ch.green;
    dst->ch.red = src->ch.red;
#endif
}

/* Todo: add gpu */
static void lcd_fb_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int x1, x2, y1, y2;

    x1 = area->x1;
    x2 = area->x2;
    y1 = area->y1;
    y2 = area->y2;

    /*Return if the area is out the screen*/
    if (x2 < 0)
        return;
    if (y2 < 0)
        return;
    if (x1 > info.width - 1)
        return;
    if (y1 > info.height - 1)
        return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > info.width - 1 ? info.width - 1 : x2;
    int32_t act_y2 = y2 > info.height - 1 ? info.height - 1 : y2;

    uint32_t x;
    uint32_t y;
    long int location = 0;

    /* 8 bit per pixel */
    if (info.bits_per_pixel == 8)
    {
        uint8_t *fbp8 = (uint8_t *)info.framebuffer;
        //TODO color convert maybe
        for (y = act_y1; y <= act_y2; y++)
        {
            for (x = act_x1; x <= act_x2; x++)
            {
                location = (x) + (y)*info.width;
                fbp8[location] = color_p->full;
                color_p++;
            }

            color_p += x2 - act_x2;
        }
    }

    /* 16 bit per pixel */
    else if (info.bits_per_pixel == 16)
    {
        lv_color16_t *fbp16 = (lv_color16_t *)info.framebuffer;

        for (y = act_y1; y <= act_y2; y++)
        {
            for (x = act_x1; x <= act_x2; x++)
            {
                location = (x) + (y)*info.width;
                color_to16_maybe(&fbp16[location], color_p);
                color_p++;
            }

            color_p += x2 - act_x2;
        }
    }

    /* 24 or 32 bit per pixel */
    else if (info.bits_per_pixel == 24 || info.bits_per_pixel == 32)
    {
        uint32_t *fbp32 = (uint32_t *)info.framebuffer;
        //TODO
        for (y = act_y1; y <= act_y2; y++)
        {
            for (x = act_x1; x <= act_x2; x++)
            {
                location = (x) + (y)*info.width;
                fbp32[location] = color_p->full;
                color_p++;
            }

            color_p += x2 - act_x2;
        }
    }

    struct rt_device_rect_info rect_info;

    rect_info.x = x1;
    rect_info.y = y1;
    rect_info.width = x2 - x1 + 1;
    rect_info.height = y2 - y1 + 1;
    rt_device_control(device, RTGRAPHIC_CTRL_RECT_UPDATE, &rect_info);

    lv_disp_flush_ready(disp_drv);
}

static void lcd_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int x1, x2, y1, y2;

    x1 = area->x1;
    x2 = area->x2;
    y1 = area->y1;
    y2 = area->y2;

    /*Return if the area is out the screen*/
    if (x2 < 0)
        return;
    if (y2 < 0)
        return;
    if (x1 > info.width - 1)
        return;
    if (y1 > info.height - 1)
        return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > info.width - 1 ? info.width - 1 : x2;
    int32_t act_y2 = y2 > info.height - 1 ? info.height - 1 : y2;

    uint32_t x;
    uint32_t y;

    //TODO color convert
    for (y = act_y1; y <= act_y2; y++)
    {
        rt_graphix_ops(device)->blit_line((const char *)color_p, act_x1, y, act_x2 - act_x1 + 1);
        color_p += (x2 - x1 + 1);
    }
    
    lv_disp_flush_ready(disp_drv);
}

static rt_bool_t touch_down = RT_FALSE;
static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static bool input_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = (touch_down == RT_TRUE) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    return false;
}

static void lvgl_tick_run(void *p)
{
    if (_lv_init)
    {
        lv_tick_inc(1);
    }
}

static int lvgl_tick_handler_init(void)
{
    rt_timer_t timer = RT_NULL;
    int ret;

    timer = rt_timer_create("lv_tick", lvgl_tick_run, RT_NULL, 1,RT_TIMER_FLAG_PERIODIC);
    if (timer == RT_NULL)
    {
        return RT_ERROR;
    }
    ret = rt_timer_start(timer);

    return ret;
}

static void lvgl_task_run(void *p)
{
    void *buf1 = disp_buf.buf1;

    while (_lv_init)
    {
        rt_thread_mdelay(10);
        lv_task_handler();
    }
#if (LV_ENABLE_GC == 1)
    lv_deinit();
#endif
    rt_free(buf1);
}

static int lvgl_task_handler_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_thread_t thread = RT_NULL;

    /* littleGL demo gui thread */
    thread = rt_thread_create("lv-task", lvgl_task_run, RT_NULL, 10 * 1024, 5, 10);
    if (thread == RT_NULL)
    {
        return RT_ERROR;
    }
    ret = rt_thread_startup(thread);

    return ret;
}

void littlevgl2rtt_send_input_event(rt_int16_t x, rt_int16_t y, rt_uint8_t state)
{
    switch (state)
    {
    case LITTLEVGL2RTT_INPUT_UP:
        touch_down = RT_FALSE;
        break;
    case LITTLEVGL2RTT_INPUT_DOWN:
        last_x = x;
        last_y = y;
        touch_down = RT_TRUE;
        break;
    case LITTLEVGL2RTT_INPUT_MOVE:
        last_x = x;
        last_y = y;
        break;
    }
}

#if USE_LV_LOG
void littlevgl2rtt_log_register(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{
    if (level >= LV_LOG_LEVEL)
    {
        //Show the log level if you want
        if (level == LV_LOG_LEVEL_TRACE)
        {
            rt_kprintf("Trace:");
        }

        rt_kprintf("%s\n", dsc);
        //You can write 'file' and 'line' too similary if required.
    }
}
#endif

rt_err_t littlevgl2rtt_init(const char *name)
{
    lv_color_t *fbuf;

    RT_ASSERT(name != RT_NULL);
    if (_lv_init == 1)
        return 0;

    /* LCD Device Init */
    device = rt_device_find(name);
    RT_ASSERT(device != RT_NULL);
    if (rt_device_open(device, RT_DEVICE_OFLAG_RDWR) == RT_EOK)
    {
        rt_device_control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    }

    RT_ASSERT(info.bits_per_pixel == 8 || info.bits_per_pixel == 16 ||
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32);
#if 0
    if ((info.bits_per_pixel != LV_COLOR_DEPTH) && (info.bits_per_pixel != 32 && LV_COLOR_DEPTH != 24))
    {
        rt_kprintf("Error: framebuffer color depth mismatch! (Should be %d to match with LV_COLOR_DEPTH)",
                   info.bits_per_pixel);
        return RT_ERROR;
    }
#endif
    fbuf = rt_malloc(info.width * 10 * sizeof(*fbuf));
    if (!fbuf)
    {
        rt_kprintf("Error: alloc disp buf fail\n");
        return -1;
    }

    /* littlevgl Init */
    lv_init();

#if USE_LV_LOG
    /* littlevgl Log Init */
    lv_log_register_print(littlevgl2rtt_log_register);
#endif

    /* littlevGL Display device interface */
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = info.width;
    disp_drv.ver_res = info.height;

    if (info.framebuffer == RT_NULL)
    {
        disp_drv.flush_cb = lcd_flush;
    }
    else
    {
        disp_drv.flush_cb = lcd_fb_flush;
    }

    lv_disp_buf_init(&disp_buf, fbuf, NULL, info.width * 10);
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = input_read;

    lv_indev_drv_register(&indev_drv);
    _lv_init = 1;

    /* littlevGL Tick thread */
    lvgl_tick_handler_init();
    lvgl_task_handler_init();

    /* Info Print */
    rt_kprintf("[littlevgl2rtt] Welcome to the littlevgl2rtt.\n");
    rt_kprintf("[littlevgl2rtt] You can find latest ver from https://github.com/liu2guang/LittlevGL2RTT.\n");

    return RT_EOK;
}

void littlevgl2rtt_deinit(void)
{
    _lv_init = 0;
}
