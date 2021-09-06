/*
 * qled.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-06-22     qiyongzhong       first version
 */
     
#include <rtthread.h>
#include <rtdevice.h>
#include <qled.h>

#define DBG_TAG "qled"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

typedef enum{
    QLED_MODE_OFF = 0,
    QLED_MODE_ON,
    QLED_MODE_BLINK,
    QLED_MODE_SPECIAL
}qled_mode_t;

typedef struct
{
    s16 pin;
    u8 mode;
    u8 level;
    u8 idx;
    u8 total;
    s16 cnt;
    u16 ton_ms;
    u16 toff_ms;
    const u16 *datas;
    void (*over_cb)(void);
}qled_data_t;

static qled_data_t qled_datas[QLED_TOTAL];

static void qled_pin_init(int pin)
{
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, PIN_LOW);
}

static void qled_pin_deinit(int pin)
{
    rt_pin_mode(pin, PIN_MODE_INPUT);
}

static void qled_datas_init(void)
{
    memset((void *)qled_datas, 0, sizeof(qled_datas));
    
    for (int i = 0; i < QLED_TOTAL; i++)
    {
        qled_datas[i].pin = -1;
    }
}

static int qled_find(int pin)
{
    for (int i = 0; i < QLED_TOTAL; i++)
    {
        if (qled_datas[i].pin == pin)
        {
            return(i);
        }
    } 
    return(-1);
}

static int qled_get_unused(void)
{
     for (int i = 0; i < QLED_TOTAL; i++)
    {
        if (qled_datas[i].pin == -1)
        {
            return(i);
        }
    } 
    return(-1);
}

static void qled_switch_on(int pin, int level)
{
    if (level)
    {
        rt_pin_write(pin, PIN_HIGH);
    }
    else
    {
        rt_pin_write(pin, PIN_LOW);
    }
}

static void qled_switch_off(int pin, int level)
{
    if (level)
    {
        rt_pin_write(pin, PIN_LOW);
    }
    else
    {
        rt_pin_write(pin, PIN_HIGH);
    }
}

static void qled_run_blink(int idx)
{
    qled_datas[idx].cnt--;
    if (qled_datas[idx].cnt <= 0)
    {
        qled_datas[idx].idx++;
        qled_datas[idx].idx %= 2;
        if (qled_datas[idx].idx == 0)
        {
            qled_datas[idx].cnt = qled_datas[idx].ton_ms / QLED_TIME_UNIT_MS;
        }
        else
        {
            qled_datas[idx].cnt = qled_datas[idx].toff_ms / QLED_TIME_UNIT_MS;
        }
    }
    if (qled_datas[idx].idx == 0)
    {
        qled_switch_on(qled_datas[idx].pin, qled_datas[idx].level);
    }
    else
    {
        qled_switch_off(qled_datas[idx].pin, qled_datas[idx].level);
    }
}

static void qled_run_special(int idx)
{
    qled_datas[idx].cnt--;
    if (qled_datas[idx].cnt <= 0)
    {
        qled_datas[idx].idx++;
        if (qled_datas[idx].idx >= qled_datas[idx].total)
        {
            qled_datas[idx].mode = QLED_MODE_OFF;
            qled_switch_off(qled_datas[idx].pin, qled_datas[idx].level);
            if (qled_datas[idx].over_cb)
            {
                (qled_datas[idx].over_cb)();
                return;
            }
        }
        qled_datas[idx].cnt = *((u16*)(qled_datas[idx].datas) + qled_datas[idx].idx) / QLED_TIME_UNIT_MS;
    }
    if ((qled_datas[idx].idx & 0x01) == 0)
    {
        qled_switch_on(qled_datas[idx].pin, qled_datas[idx].level);
    }
    else
    {
        qled_switch_off(qled_datas[idx].pin, qled_datas[idx].level);
    }
}

static void qled_run(void)
{
    rt_enter_critical();
    
    for (int i = 0; i < QLED_TOTAL; i++)
    {
        if (qled_datas[i].pin == -1)
        {
            continue;
        }
        
        switch(qled_datas[i].mode)
        {
        case QLED_MODE_OFF:
            qled_switch_off(qled_datas[i].pin, qled_datas[i].level);
            break;
        case QLED_MODE_ON:
            qled_switch_on(qled_datas[i].pin, qled_datas[i].level);
            break;
        case QLED_MODE_BLINK:
            qled_run_blink(i);
            break;
        case QLED_MODE_SPECIAL:
            qled_run_special(i);
            break;
        default:
            break;
        }
    }
    
    rt_exit_critical();
}

static void qled_thread_entry(void *params)
{
    qled_datas_init();

    while(1)
    {
        qled_run();
        rt_thread_mdelay(QLED_TIME_UNIT_MS);
    }
}

static void qled_init(void)
{
    rt_thread_t tid = rt_thread_create(QLED_THREAD_NAME, 
                                        qled_thread_entry, 
                                        NULL, 
                                        QLED_THREAD_STACK_SIZE, 
                                        QLED_THREAD_PRIO, 
                                        20);
    rt_thread_startup(tid);
}
INIT_PREV_EXPORT(qled_init);


int qled_add(int pin, int level)
{
    int idx;

    if (pin < 0)
    {
        LOG_E("led add fail. param pin is error.");
        return(-RT_ERROR);
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        idx = qled_get_unused();
    }

    if (idx < 0)
    {
        LOG_E("led add fail. led space is full.");
        return(-RT_ERROR);
    }
    
    qled_pin_init(pin);
    qled_switch_off(pin, (level != 0));
    
    rt_enter_critical();
    
    qled_datas[idx].pin = pin;
    qled_datas[idx].level = (level != 0);
    
    rt_exit_critical();

    return(RT_EOK);
}

void qled_remove(int pin)
{
    int idx;

    if (pin < 0)
    {
        return;
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        return;
    }
    
    qled_pin_deinit(pin);
    
    qled_datas[idx].pin = -1;
    
}

int qled_set_off(int pin)
{
    int idx;

    if (pin < 0)
    {
        LOG_E("led add fail. param pin is error.");
        return(-RT_ERROR);
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        LOG_E("led set mode off fail. pin is not found.");
        return(-RT_ERROR);
    }
    
    qled_datas[idx].mode = QLED_MODE_OFF;
    qled_switch_off(pin, qled_datas[idx].level);
    
    return(RT_EOK);
}

int qled_set_on(int pin)
{
    int idx;

    if (pin < 0)
    {
        LOG_E("led add fail. param pin is error.");
        return(-RT_ERROR);
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        LOG_E("led set mode on fail. pin is not found.");
        return(-RT_ERROR);
    }
    
    qled_datas[idx].mode = QLED_MODE_ON;
    qled_switch_on(pin, qled_datas[idx].level);
    
    return(RT_EOK);
}

int qled_set_blink(int pin, int ton_ms, int toff_ms)
{
    int idx;

    if (pin < 0)
    {
        LOG_E("led add fail. param pin is error.");
        return(-RT_ERROR);
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        LOG_E("led set mode blink fail. pin is not found.");
        return(-RT_ERROR);
    }

    rt_enter_critical();

    qled_datas[idx].cnt = 0;
    qled_datas[idx].idx = -1;
    qled_datas[idx].ton_ms = ton_ms;
    qled_datas[idx].toff_ms = toff_ms;
    qled_datas[idx].mode = QLED_MODE_BLINK;
    qled_run_blink(idx);
    
    rt_exit_critical();
    
    return(RT_EOK);
}

int qled_set_special(int pin, const u16 *datas, int data_total, void (*over_cb)(void))
{
    int idx;

    if (pin < 0)
    {
        LOG_E("led add fail. param pin is error.");
        return(-RT_ERROR);
    }
    
    idx = qled_find(pin);
    if (idx < 0)
    {
        LOG_E("led set mode blink fail. pin is not found.");
        return(-RT_ERROR);
    }

    rt_enter_critical();
    
    qled_datas[idx].cnt = 0;
    qled_datas[idx].idx = -1;
    qled_datas[idx].datas = datas;
    qled_datas[idx].total = data_total;
    qled_datas[idx].over_cb = over_cb;
    qled_datas[idx].mode = QLED_MODE_SPECIAL;
    qled_run_special(idx);
    
    rt_exit_critical();
    
    return(RT_EOK);
}

