#include <js_demo_port.h>

extern void FPS_CPU_ENABLE(int s);
static int FPS_CPU = 0;

int open_fps_display(char value)
{
    FPS_CPU_ENABLE(value);
    FPS_CPU = value;

    if (FPS_CPU == 0)
        return jerry_create_boolean(false);
    else
        return jerry_create_boolean(true);
}

DECLARE_HANDLER(FPS_CPU_GET)
{
    if (FPS_CPU == 0)
        return jerry_create_boolean(false);
    else
        return jerry_create_boolean(true);
}

DECLARE_HANDLER(FPS_CPU_ENABLE)
{
    if (args_cnt >= 1 && jerry_value_is_number(args[0]))
    {
        int value = (int)jerry_get_number_value(args[0]);
        FPS_CPU_ENABLE(value);
        FPS_CPU = value;
        return jerry_create_boolean(true);
    }
    else
    {
        return jerry_create_boolean(false);
    }
}

static int PM_REQUEST = 0;

DECLARE_HANDLER(PM_REQUEST_GET)
{
    if (PM_REQUEST == 0)
        return jerry_create_boolean(true);
    else
        return jerry_create_boolean(false);
}

DECLARE_HANDLER(PM_REQUEST)
{
    if (args_cnt >= 1 && jerry_value_is_number(args[0]))
    {
        int value = (int)jerry_get_number_value(args[0]);
        if (value)
        {
            if (PM_REQUEST == 0)
            {
                //rt_pm_run_request();
                PM_REQUEST = 1;
            }
        }
        else
        {
            if (PM_REQUEST == 1)
            {
                //rt_pm_run_release();
                PM_REQUEST = 0;
            }
        }

        return jerry_create_boolean(true);
    }
    else
    {
        return jerry_create_boolean(false);
    }
}

int js_demo_init(void)
{
    rt_kprintf("==> js_demo_init\n");
    REGISTER_HANDLER(FPS_CPU_ENABLE);
    REGISTER_HANDLER(FPS_CPU_GET);
    REGISTER_HANDLER(PM_REQUEST);
    REGISTER_HANDLER(PM_REQUEST_GET);

    return 0;
}

/*
JS 示例代码：

var value = hq_get();
console.dir(value);

value = 100;
hq_set(value);
value = hq_get();
console.dir(value);

var hq_sys = hq_system();
console.dir(hq_sys);

value = hq_sys.hq_get();
console.dir(value);

value = 200;
hq_sys.hq_set(value);
value = hq_sys.hq_get();
console.dir(value);

运行结果 LOG：

==> hq_get
0.000000

==> hq_set
==> hq_get
100.000000

==> hq_system
{hq_set : [function], hq_get : [function]}

==> hq_get
100.000000

==> hq_set
==> hq_get
200.000000

*/
