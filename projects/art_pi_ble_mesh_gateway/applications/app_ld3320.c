#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#define DBG_TAG "ld3320"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "ld3320.h"
#include "drv_spi.h"
#include "drv_common.h"
#include "drv_gpio.h"
#include "mesh_command.h"

#define THREAD_STACK_SIZE 1024
#define LD3320_SC_PIN GET_PIN(H, 3)
#define LD3320_WR_PIN GET_PIN(B, 0)
#define LD3320_RST_PIN GET_PIN(B, 2)
#define LD3320_IRQ_PIN GET_PIN(B, 1)

extern struct rt_messagequeue player_mq;
extern rt_mutex_t player_use_mtx;
extern void mesh_send_command(command_opcode_t opc, uint16_t addr, uint8_t *data);

static void send_paly_task(char c)
{
    int result;

    result = rt_mq_urgent(&player_mq, &c, 1);
    if (result != RT_EOK)
    {
        rt_kprintf("rt_mq_urgent ERR\n");
    }
}

static void ld3320_a_asr_over_callback(uint8_t num)
{
    uint8_t mesh_command[COMMAND_DATA_LENGTH];
    switch (num)
    {
    case 1:
        rt_kprintf("我在");
        send_paly_task('1');
        break;
    case 2:
        rt_kprintf("开灯");
        mesh_command[0] = 1;
        mesh_send_command(COMMAND_OPCODE_SET, 0xC023, mesh_command);
        send_paly_task('2');
        break;
    case 3:
        rt_kprintf("关灯");
        mesh_command[0] = 0;
        mesh_send_command(COMMAND_OPCODE_SET, 0xC023, mesh_command);
        send_paly_task('3');
        break;
    case 4:
        rt_kprintf("搜索设备");
        mesh_send_command(COMMAND_OPCODE_FIND, 0, NULL);
        send_paly_task('2');
        break;
    default:
        break;
    }
}

static void ld3320_asr_thread(void *parameter)
{
    static ld3320_t _ld3320;

    rt_pin_mode(LD3320_SC_PIN, PIN_MODE_OUTPUT);

    rt_hw_spi_device_attach("spi4", "spi40", GPIOH, GPIO_PIN_3);

    _ld3320 = ld3320_create("spi40", LD3320_WR_PIN, LD3320_RST_PIN, LD3320_IRQ_PIN, LD3320_MODE_ASR);
    ld3320_set_asr_over_callback(_ld3320, ld3320_a_asr_over_callback);

    ld3320_addcommand_tolist(_ld3320, "xiao la jiao", 1);
    ld3320_addcommand_tolist(_ld3320, "kai deng", 2);
    ld3320_addcommand_tolist(_ld3320, "guan deng", 3);
    ld3320_addcommand_tolist(_ld3320, "sou suo she bei", 4);
    ld3320_addcommand_fromlist(_ld3320);

    ld3320_asr_start(_ld3320);
    while (1)
    {
        rt_mutex_take(player_use_mtx, RT_WAITING_FOREVER);
        rt_kprintf("识别中...\n");
        // rt_pin_write(LED_G, 0);
        ld3320_run(_ld3320, LD3320_MODE_ASR);
        rt_kprintf("识别完成\n");
        // rt_pin_write(LED_G, 1);
        rt_mutex_release(player_use_mtx);
        rt_thread_mdelay(100);
    }
}

static int create_ld3320_asr_thread(void)
{
    rt_thread_t thread = RT_NULL;
    thread = rt_thread_create("ld_asr", ld3320_asr_thread, RT_NULL, THREAD_STACK_SIZE, 15, 100);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    return RT_EOK;
}

INIT_APP_EXPORT(create_ld3320_asr_thread);
