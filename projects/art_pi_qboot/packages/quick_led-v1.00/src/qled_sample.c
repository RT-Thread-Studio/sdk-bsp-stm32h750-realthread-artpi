/*
 * qled_sample.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-06-22     qiyongzhong       first version
 */
      
#include <rtthread.h>
#include <qled_sample.h>

#ifdef QLED_USING_SAMPLE

#if (QLED_RUN_PIN >= 0)
static void qled_run_init(void)
{
    qled_add(QLED_RUN_PIN, 1);
    qled_set_blink(QLED_RUN_PIN, 50, 450);
}
INIT_DEVICE_EXPORT(qled_run_init);
#endif

#if (QLED_SOS_PIN >= 0)
static int sos_send_times = 0;
static const u16 sos_datas[] = 
{
    200, 200, 200, 200, 200, 200,       //short 3 times
    600, 600, 600, 600, 600, 600,       //long 3 times
    200, 200, 200, 200, 200, 200 + 2000 //short 3 times and 2000ms interval
};

static void qled_sos_cb(void)
{
    sos_send_times--;
    if (sos_send_times > 0)
    {
        qled_set_special(QLED_SOS_PIN, sos_datas, sizeof(sos_datas)/sizeof(u16), qled_sos_cb);
    }
    else
    {
        qled_remove(QLED_SOS_PIN);
    }
}

static void qled_send_sos(void)//send 5 times sos signal, total 40 s
{
    sos_send_times = 5;
    qled_add(QLED_SOS_PIN, 1);
    qled_set_special(QLED_SOS_PIN, sos_datas, sizeof(sos_datas)/sizeof(u16), qled_sos_cb);
}
MSH_CMD_EXPORT_ALIAS(qled_send_sos, led_sos, qled send sos to test special mode.)
#endif

#endif

