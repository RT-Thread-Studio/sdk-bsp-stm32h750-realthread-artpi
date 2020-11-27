
#include "jerry_util.h"
#include "rtthread.h"
#include <jerry_message.h>
#include <js_demo_port.h>
#include <ecma-globals.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME    "JS_USER"
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

/* Message.send 鎺ュ彛瀵规帴鐨勫鐞嗗嚱鏁� */
static void js_message_rev_func(const char *name, rt_uint8_t *data, rt_uint32_t size)
{
    LOG_I("js_message name: %s", name);
}

/* 鐢ㄦ埛瀹炵幇 JS 鎺ュ彛鍒濆鍖栧叆鍙� */
static void _js_util_user_init(void)
{
    js_demo_init(); //JS demo 鎺ュ彛鍒濆鍖�
}

/* 鐢ㄦ埛瀹炵幇 JS 鎺ュ彛閲婃斁娓呴櫎鍏ュ彛 */
static void _js_util_user_cleanup(void)
{

}

static rt_timer_t one_sec_tick = NULL;
static rt_tick_t time_sec = 0;

static void _one_sec_tick_timeout(void *parameter)
{
    if (time_sec >= 1)
    {
        int arg = 1000;
        rt_timer_control(one_sec_tick, RT_TIMER_CTRL_SET_TIME, &arg);
        if (js_util_lock() == RT_EOK)
        {
            jerry_value_t value = jerry_create_number(time_sec);
            js_message_send_data("sec_tick", value);
            jerry_release_value(value);
            js_util_unlock();
        }
        time_sec = 0;
    }
    else
        js_message_send_data("sec_tick", ECMA_VALUE_UNDEFINED);
}

void sec_tick_enable(int s)
{
    if (one_sec_tick)
    {
        if (s)
        {
            time_sec = (rt_tick_get() - time_sec) / 1000;
            if (time_sec >= 0)
            {
                int arg = 10;
                rt_timer_control(one_sec_tick, RT_TIMER_CTRL_SET_TIME, &arg);
            }
            rt_timer_start(one_sec_tick);
        }
        else
        {
            time_sec = rt_tick_get();
            rt_timer_stop(one_sec_tick);
        }
    }
}

/* JS 鐢ㄦ埛鍒濆鍖栧姛鑳藉叆鍙ｅ嚱鏁帮紝闇�瑕佸湪 JS 寮曟搸鍒濆鍖栧墠璋冪敤 */
int js_user_init(void)
{
    rt_kprintf("==> js_user_init\n");
    js_message_send_func_init(&js_message_rev_func);//娉ㄥ唽 Message.send 鎺ュ彛瀵规帴鐨勫鐞嗗嚱鏁�
    js_util_user_init(&_js_util_user_init);         //娉ㄥ唽鐢ㄦ埛瀹炵幇 JS 鎺ュ彛鍒濆鍖栧叆鍙ｅ嚱鏁�
    js_util_user_cleanup(&_js_util_user_cleanup);   //娉ㄥ唽鐢ㄦ埛瀹炵幇 JS 鎺ュ彛閲婃斁娓呴櫎鍏ュ彛鍑芥暟

    one_sec_tick = rt_timer_create("sec_tick", _one_sec_tick_timeout, NULL,
            RT_TICK_PER_SECOND, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    rt_timer_start(one_sec_tick);

    return 0;
}
//INIT_COMPONENT_EXPORT(js_user_init);

#include "finsh.h"

int gui_sleep(int argc, char **argv)
{
    if (argc >= 2)
    {
        int v = atoi(argv[1]);
        if (v)
        {
            sec_tick_enable(0);
        }
        else
        {
            sec_tick_enable(1);
        }
    }
    return 0;
}
MSH_CMD_EXPORT(gui_sleep, gui_sleep);
