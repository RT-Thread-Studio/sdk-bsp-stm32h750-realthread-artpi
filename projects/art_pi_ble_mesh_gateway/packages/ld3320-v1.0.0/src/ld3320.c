/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-03     xqyjlj       the first version
 */
#include "ld3320.h"


#define DBG_TAG "ld3320"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#ifdef PKG_USING_LD3320_MP3
#include <dfs_posix.h>
#endif

static rt_sem_t ld3320_sem = RT_NULL;

void rt_hw_us_delay(rt_uint32_t us);

/**
 * @name:    ld3320_soft_rst
 * @brief:   ld3320 software reset
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
static void ld3320_soft_rst(ld3320_t ops)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0x17, 0x35);/* ld3320 software reset */
}
/**
 * @name:    ld3320_init_common
 * @brief:   ld3320 general initialization
 * @param:
 *           ops:   ld3320_t handle
 *           mode:  run mode
 * @retval:
 *           None
 */
static void ld3320_init_common(ld3320_t ops, uint8_t mode)
{
    RT_ASSERT(ops);
    ld3320_read_reg(ops->obj.dev, 0x06);/* read fifo status */
    ld3320_soft_rst(ops);/* software reset */
    rt_hw_us_delay(10);
    ld3320_read_reg(ops->obj.dev, 0x06);/* read fifo status */

    ld3320_write_reg(ops->obj.dev, 0x89, 0x03);/* analog circuit control initialization */
    rt_hw_us_delay(10);
    ld3320_write_reg(ops->obj.dev, 0xcf, 0x43);/* internal power saving mode initialization */

    rt_hw_us_delay(10);
    ld3320_write_reg(ops->obj.dev, 0xcb, 0x02);/* read ASR results (alternate 4)*/

    /*PLL setting*/
    ld3320_write_reg(ops->obj.dev, 0x11, LD3320_PLL_11);/* set clock frequency 1 */
    if (mode == LD3320_MODE_ASR)
    {
        ld3320_write_reg(ops->obj.dev, 0x1e, 0x00);/* ADC control initialization */
        ld3320_write_reg(ops->obj.dev, 0x19, LD3320_PLL_ASR_19);/* set clock frequency 2 */
        ld3320_write_reg(ops->obj.dev, 0x1b, LD3320_PLL_ASR_1B);/* set clock frequency 3 */
        ld3320_write_reg(ops->obj.dev, 0x1d, LD3320_PLL_ASR_1D);/* set clock frequency 4 */

    }
#ifdef PKG_USING_LD3320_MP3
    else if (mode == LD3320_MODE_MP3)
    {
        ld3320_write_reg(ops->obj.dev, 0x1e, 0x00);/* ADC control initialization */
        ld3320_write_reg(ops->obj.dev, 0x19, LD3320_PLL_MP3_19);/* set clock frequency 2 */
        ld3320_write_reg(ops->obj.dev, 0x1b, LD3320_PLL_MP3_1B);/* set clock frequency 3 */
        ld3320_write_reg(ops->obj.dev, 0x1d, LD3320_PLL_MP3_1D);/* set clock frequency 4 */
    }
#endif
    rt_hw_us_delay(1);
    ld3320_write_reg(ops->obj.dev, 0xcd, 0x04);/* allow DSP to sleep */
    ld3320_write_reg(ops->obj.dev, 0x17, 0x4c);/* DSP sleep */
    rt_hw_us_delay(10);
    ld3320_write_reg(ops->obj.dev, 0xb9, 0x00);/* ASR string length initialization */
    ld3320_write_reg(ops->obj.dev, 0xcf, 0x4f);/* internal power saving mode (initialize MP3 and ASR) */
    ld3320_write_reg(ops->obj.dev, 0x6f, 0xff);/* initialize chip */
}
/**
 * @name:    ld3320_init_asr
 * @brief:   Initialize LD3320 ASR mode
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
static void ld3320_init_asr(ld3320_t ops)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xbd, 0x00);/* initialize the ASR controller */
    ld3320_write_reg(ops->obj.dev, 0x17, 0x48);/* activate DSP */
    rt_hw_us_delay(1);
    ld3320_write_reg(ops->obj.dev, 0x3c, 0x80);/* initialize FIFO EXT */
    ld3320_write_reg(ops->obj.dev, 0x3e, 0x07);/* initialize FIFO EXT */
    ld3320_write_reg(ops->obj.dev, 0x38, 0xff);/* initialize FIFO EXT */
    ld3320_write_reg(ops->obj.dev, 0x3a, 0x07);/* initialize FIFO EXT */
    ld3320_write_reg(ops->obj.dev, 0x40, 0x00);/* initialize FIFO EXT MCU */
    ld3320_write_reg(ops->obj.dev, 0x42, 0x08);/* initialize FIFO EXT MCU */
    ld3320_write_reg(ops->obj.dev, 0x44, 0x00);/* initialize FIFO EXT DSP */
    ld3320_write_reg(ops->obj.dev, 0x46, 0x08);/* initialize FIFO EXT DSP */
    rt_hw_us_delay(1);
}

#ifdef PKG_USING_LD3320_MP3
/**
 * @name:    ld3320_init_mp3
 * @brief:   Initialize LD3320 MP3 mode
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
static void ld3320_init_mp3(ld3320_t ops)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xBD, 0x02);/* initialize the MP3 controller */
    ld3320_write_reg(ops->obj.dev, 0x17, 0x48);/* activate DSP */
    rt_hw_us_delay(10);

    ld3320_write_reg(ops->obj.dev, 0x85, 0x52);/* initialize internal feedback */
    ld3320_write_reg(ops->obj.dev, 0x8F, 0x00);/* initialize LineOut selection */
    ld3320_write_reg(ops->obj.dev, 0x81, 0x00);/* initialize earphone left volume */
    ld3320_write_reg(ops->obj.dev, 0x83, 0x00);/* initialize earphone right volume */
    ld3320_write_reg(ops->obj.dev, 0x8E, 0xff);/* initialize speaker output volume */
    ld3320_write_reg(ops->obj.dev, 0x8D, 0xff);/* initialize internal gain control */
    rt_hw_us_delay(10);

    ld3320_write_reg(ops->obj.dev, 0x87, 0xff);/* initialize analog circuit control */
    ld3320_write_reg(ops->obj.dev, 0x89, 0xff);/* initialize MP3 analog circuit control */
    rt_hw_us_delay(10);

    ld3320_write_reg(ops->obj.dev, 0x22, 0x00);/* initialize FIFO DATA */
    ld3320_write_reg(ops->obj.dev, 0x23, 0x00);/* initialize FIFO DATA */
    ld3320_write_reg(ops->obj.dev, 0x20, 0xef);/* initialize FIFO DATA */
    ld3320_write_reg(ops->obj.dev, 0x21, 0x07);/* initialize FIFO DATA */
    ld3320_write_reg(ops->obj.dev, 0x24, 0x77);/* initialize FIFO DATA MCU */
    ld3320_write_reg(ops->obj.dev, 0x25, 0x03);/* initialize FIFO DATA MCU */
    ld3320_write_reg(ops->obj.dev, 0x26, 0xbb);/* initialize FIFO DATA DSP */
    ld3320_write_reg(ops->obj.dev, 0x27, 0x01);/* initialize FIFO DATA DSP */
}
#endif


#ifdef PKG_USING_LD3320_MP3
/**
 * @name:    ld3320_set_mix2spVolume
 * @brief:
 * @param:
 *           ops:   ld3320_t handle
 *           val:   volume
 * @retval:
 *           None
 */
static void ld3320_set_mix2spVolume(ld3320_t ops, uint8_t val)
{
    val = ((32 - val) & 0x0f) << 2;
    ld3320_write_reg(ops->obj.dev, 0x8E, val | 0xc3);
    ld3320_write_reg(ops->obj.dev, 0x87, 0x78);
}
#endif


/**
 * @name:    ld3320_check_asrbusy_flag_b2
 * @brief:   check whether the value of LD3320 b2 register is equal to 0x21, only when it is equal to 0x21, LD3320 is idle
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           1：ld3320 idle
 *           0：LD3320 busy
 */
static uint8_t ld3320_check_asrbusy_flag_b2(ld3320_t ops)
{
    RT_ASSERT(ops);
    uint8_t j;
    uint8_t flag = 0;

    for (j = 0; j < 10; j++)
    {
        if (ld3320_read_reg(ops->obj.dev, 0xb2) == 0x21)
        {
            flag = 1;
            break;
        }
        rt_hw_us_delay(10);
    }
    return flag;
}


/**
 * @name:    ld3320_set_speechEndpoint
 * @brief:   adjust the end time of the voice endpoint (interval time between words), parameters (0x00~0xC3, unit 10MS)
 * @param:
 *           ops:               ld3320_t handle
 *           speech_endpoint_:  voice endpoint end time
 * @retval:
 *           None
 */
static void ld3320_set_speechEndpoint(ld3320_t ops, uint8_t speech_endpoint)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xb3, speech_endpoint);
}


/**
 * @name:    ld3320_set_micvol
 * @brief:   ddjust ADC gain, parameters (0x00~0xFF, recommended 10-60)
 * @param:
 *           ops:       ld3320_t handle
 *           micvol:    ADC gain
 * @retval:
 *           None
 */
static void ld3320_set_micvol(ld3320_t ops, uint8_t micvol)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0x35, micvol);
}


/**
 * @name:    ld3320_speechStartTime
 * @brief:   adjust the start time of the voice endpoint, parameters (0x00~0x30, unit 10MS)
 * @param:
 *           ops:                   ld3320_t handle
 *           speech_start_time_:    Voice endpoint start time
 * @retval:
 *           None
 */
static void ld3320_set_speechStartTime(ld3320_t ops,uint8_t speech_start_time)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xb4, speech_start_time);
}


/**
 * @name:    ld3320_speechEndTime
 * @brief:   adjust the end time of the voice endpoint (interval time of utterance), parameters (0x00~0xC3, unit 10MS);
 * @param:
 *           ops:               ld3320_t handle
 *           speech_end_time:   voice end time
 * @retval:
 *           None
 */
static void ld3320_set_speechEndTime(ld3320_t ops, uint8_t speech_end_time)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xb5, speech_end_time);
}


/**
 * @name:    ld3320_voiceMaxLength
 * @brief:   the longest voice segment time, parameter (0x00~0xC3, unit 100MS)
 * @param:
 *           ops:               ld3320_t handle
 *           speech_end_time:   the longest voice segment time
 * @retval:
 *           None
 */
static void ld3320_set_voiceMaxLength(ld3320_t ops, uint8_t voice_max_length)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xb6, voice_max_length);
}


/**
 * @name:    ld3320_noiseTime
 * @brief:   Power-on noise skip, parameter (0x00~0xff, unit 20MS)
 * @param:
 *           ops:               ld3320_t handle
 *           speech_end_time:   noise time
 * @retval:
 *           None
 */
static void ld3320_set_noiseTime(ld3320_t ops, uint8_t noise_time)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0xb7, noise_time);
}


/**
 * @name:    ld3320_asr_start
 * @brief:   start LD3320 ASR
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           1：start success
 *           0：start failure
 */
uint8_t ld3320_asr_start(ld3320_t ops)
{
    RT_ASSERT(ops);
    ld3320_write_reg(ops->obj.dev, 0x1c, 0x09);/* ADC switch control, Reserved */
    ld3320_write_reg(ops->obj.dev, 0xbd, 0x20);/* control mode  */
    ld3320_write_reg(ops->obj.dev, 0x08, 0x01);/* clear FIFO DATA */
    rt_hw_us_delay(1);

    ld3320_write_reg(ops->obj.dev, 0x08, 0x00);/* need to write 0x00 again after clearing the FIFO */
    rt_hw_us_delay(1);

    if (ld3320_check_asrbusy_flag_b2(ops) == 0)
    {
        return 0;
    }

    ld3320_write_reg(ops->obj.dev, 0xb2, 0xff);/* DSP busy state */
    ld3320_write_reg(ops->obj.dev, 0x37, 0x06);/* notify ASR to start recognizing voice */
    rt_hw_us_delay(5);

    ld3320_write_reg(ops->obj.dev, 0x1c, 0x0b);/* microphone input ADC channel */
    ld3320_write_reg(ops->obj.dev, 0x29, 0x10);/* FIFO interrupt enable */
    ld3320_write_reg(ops->obj.dev, 0xbd, 0x00);/* activate in ASR mode */

    return 1;
}


/**
 * @name:    ld3320_addcommand
 * @brief:   add recognition sentences to ld3320
 * @param:
 *           ops:       ld3320_t handle
 *           *pass：    voice keywords
 *           num:       keyword num
 * @retval:
 *           None
 */
static void ld3320_addcommand(ld3320_t ops, char *pass, int num)
{
    RT_ASSERT(ops);
    int i;
    if (ld3320_check_asrbusy_flag_b2(ops) != 1) return;
    ld3320_write_reg(ops->obj.dev, 0xc1, num); /* character number */
    ld3320_write_reg(ops->obj.dev, 0xc3, 0); /* enter 00 when adding */
    ld3320_write_reg(ops->obj.dev, 0x08, 0x04); /* clear FIFO_EXT */

    rt_hw_us_delay(1);
    ld3320_write_reg(ops->obj.dev, 0x08, 0x00); /* need to write a 0x00 after clearing the FIFO */
    rt_hw_us_delay(1);
    for (i = 0; i <= 80; i++)
    {
        if (pass[i] == 0) break;
        ld3320_write_reg(ops->obj.dev, 0x5, pass[i]); /* write FIFO_EXT */
    }
    ld3320_write_reg(ops->obj.dev, 0xb9, i); /* write the length of the currently added string */
    ld3320_write_reg(ops->obj.dev, 0xb2, 0xff); /* B2 all write ff */
    ld3320_write_reg(ops->obj.dev, 0x37, 0x04); /* add statement */
}


/**
 * @name:    ld3320_addcommand_tonode
 * @brief:   Add recognition sentences to the ld3320 linked list structure
 * @param:
 *           ops:        ld3320_t handle
 *           *pass：     voice keywords
 *           micvol:     keyword num
 * @retval:
 *           None
 */
void ld3320_addcommand_tolist(ld3320_t ops, char *pass, int num)
{
    ld3320_command_t node;
    node = (ld3320_command_t) rt_malloc(sizeof(struct ld3320_command));
    if (node == RT_NULL)
    {
        LOG_E("fail malloc node");
        return;
    }
    rt_strncpy(node->name, pass, rt_strlen(pass) + 1);
    node->num = num;
    rt_list_insert_after(&ops->node, &node->list);
}


/**
 * @name:    ld3320_addcommand_fromnode
 * @brief:   add the recognition sentence stored in the ld3320 linked list structure into the LD3320
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
void ld3320_addcommand_fromlist(ld3320_t ops)
{
    ld3320_command_t tmp;
    rt_list_t  *node;
    node = ops->node.next;
    for (; node != &(ops->node); node = node->next)
    {
        tmp = rt_list_entry(node, struct ld3320_command, list);
        ld3320_addcommand(ops, tmp->name, tmp->num);
    }
}


/**
 * @name:    ld3320_init_chip
 * @brief:   ld3320 initialization chip
 * @param:
 *           ops:   ld3320_t handle
 *           mode:  mode
 * @retval:
 *           None
 */
static void ld3320_init_chip(ld3320_t ops, uint8_t mode)
{
    RT_ASSERT(ops);

    ld3320_init_common(ops, mode);

    if (mode == LD3320_MODE_ASR)
    {
        ld3320_init_asr(ops);
        ld3320_set_micvol(ops, LD3320_MIC_VOL);
        ld3320_set_speechEndpoint(ops, LD3320_SPEECH_END_POINT);
        ld3320_set_speechStartTime(ops, LD3320_SPEECH_START_TIME);
        ld3320_set_speechEndTime(ops, LD3320_SPEECH_END_TIME);
        ld3320_set_voiceMaxLength(ops, LD3320_VOICE_MAX_LENGTH);
        ld3320_set_noiseTime(ops, LD3320_NOISE_TIME);
    }
#ifdef PKG_USING_LD3320_MP3
    else if (mode == LD3320_MODE_MP3)
    {
        ld3320_init_mp3(ops);
        ld3320_set_mix2spVolume(ops, LD3320_MIX2SPVOLUME);
    }
#endif
}


/**
 * @name:    ld3320_hw_rst
 * @brief:   ld3320 hardware reset
 * @param:
 *           ops:  ld3320_t handle
 * @retval:
 *           None
 */
void ld3320_hw_rst(ld3320_t ops)
{
    RT_ASSERT(ops);
    if (ops->obj.port.rst_pin != LD3320_PIN_NONE)
    {
        rt_pin_write(ops->obj.port.rst_pin, PIN_LOW);
        rt_hw_us_delay(10);
        rt_pin_write(ops->obj.port.rst_pin, PIN_HIGH);
        rt_hw_us_delay(10);
    }

}


/**
 * @name:    ld3320_asr_down
 * @brief:   ld3320 interrupt signal is coming
 * @param:
 *           None
 * @retval:
 *           None
 */
static void ld3320_irq_down(void *asr)
{
    rt_sem_release(ld3320_sem); /* Release binary semaphore */
}


/**
 * @name:    ld3320_create
 * @brief:   create an ld3320 object
 * @param:
 *           spi_dev_name:   spi device name
 *           wr_pin：        WR IO port, if not set to LD3320_PIN_NONE
 *           rst_pin：       RST IO port, if not set to LD3320_PIN_NONE
 *           irq_pin：       IRQ IO port
 *           mode：          LD3320 operating mode selection, there are two options LD3320_MODE_ASR and LD3320_MODE_MP3 to choose from
 * @retval:
 *           ld3320 object
 */
void ld3320_finsh_init(ld3320_t ops);
ld3320_t ld3320_create(char *spi_dev_name, int wr, int rst, int irq, uint8_t mode)
{
    RT_ASSERT(spi_dev_name);
    ld3320_t ops;
    /* initialization of linked list header */
    ops = (ld3320_t) rt_malloc(sizeof(struct ld3320));
    if (ops == RT_NULL)
    {
        LOG_E("ld3320 head create fail");
        return RT_NULL;
    }
    /* initialize the linked list structure */
    rt_list_init(&ops->node);
    if (!rt_list_isempty(&ops->node))
    {
        LOG_E("fail init ld3320 head");
        return RT_NULL;
    }
    /* find SPI device */
    ops->obj.dev = (struct rt_spi_device *) rt_device_find(spi_dev_name);
    if (ops->obj.dev == RT_NULL)
    {
        LOG_E("Can't find dev for ld3320 device on '%s' ", spi_dev_name);
        return RT_NULL;
    }
    /* reconfigure SPI mode */
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_2 | RT_SPI_MSB;
    cfg.max_hz = 1 * 1000 * 1000;
    rt_spi_configure(ops->obj.dev, &cfg);
    /* initialize the IO port */
    ops->obj.port.wr_pin = wr;
    if (ops->obj.port.wr_pin != LD3320_PIN_NONE)
    {
        rt_pin_mode(ops->obj.port.wr_pin, PIN_MODE_OUTPUT);
        rt_pin_write(ops->obj.port.wr_pin, PIN_LOW);
    }

    ops->obj.port.rst_pin = rst;
    if (ops->obj.port.rst_pin != LD3320_PIN_NONE)
    {
        rt_pin_mode(ops->obj.port.rst_pin, PIN_MODE_OUTPUT);
        rt_pin_write(ops->obj.port.rst_pin, PIN_HIGH);
    }

    ops->obj.port.irq_pin = irq;
    if (ops->obj.port.irq_pin != LD3320_PIN_NONE)
    {
        /* interrupt mode (falling edge trigger) */
        rt_pin_mode(ops->obj.port.irq_pin, PIN_MODE_INPUT_PULLUP);
        rt_pin_attach_irq(ops->obj.port.irq_pin, PIN_IRQ_MODE_FALLING, ld3320_irq_down, RT_NULL);
        rt_pin_irq_enable(ops->obj.port.irq_pin, PIN_IRQ_ENABLE);
        ld3320_sem = rt_sem_create("ld3320", 0, RT_IPC_FLAG_FIFO);/* binary semaphore */
        if (ld3320_sem == RT_NULL)
        {
            LOG_E("ld3320 sem create fail");
            return RT_NULL;
        }
    }
    else
    {
        LOG_E("please input correct pin");
        return RT_NULL;
    }

    ld3320_hw_rst(ops);
    ld3320_init_chip(ops, mode);
#ifdef PKG_USING_LD3320_FINSH
    ld3320_finsh_init(ops);
#endif
    return ops;
}


/**
 * @name:    ld3320_asr_run
 * @brief:   LD3320 ASR mode operation function
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
static void ld3320_asr_run(ld3320_t ops)
{
    RT_ASSERT(ops);
    uint8_t Asr_Count = 0;
    uint8_t num = 0;
    if (ops->obj.port.irq_pin != LD3320_PIN_NONE)
    {
        rt_sem_take(ld3320_sem, RT_WAITING_FOREVER); /* waiting time: wait all the time */
    }
    /* if speech recognition is interrupted, DSP is idle, ASR ends normally */
    if ((ld3320_read_reg(ops->obj.dev, 0x2b) & 0x10) && ld3320_read_reg(ops->obj.dev, 0xb2) == 0x21
            && ld3320_read_reg(ops->obj.dev, 0xbf) == 0x35)
    {
        ld3320_write_reg(ops->obj.dev, 0x29, 0); /* turn off interrupt */
        ld3320_write_reg(ops->obj.dev, 0x02, 0); /* turn off FIFO interrupt */
        Asr_Count = ld3320_read_reg(ops->obj.dev, 0xba); /* read interrupt auxiliary information */
        /* if there is a recognition result */
        if (Asr_Count > 0 && Asr_Count < 4)
        {
            num = ld3320_read_reg(ops->obj.dev, 0xc5);
            ops->obj.asr_over_callback_t(num);
        }
        ld3320_write_reg(ops->obj.dev, 0x2b, 0); /* clear interrupt number */
        ld3320_write_reg(ops->obj.dev, 0x1C, 0); /* turn off the microphone */
        ld3320_write_reg(ops->obj.dev, 0x29, 0); /* turn off interrupt */
        ld3320_write_reg(ops->obj.dev, 0x02, 0); /* turn off FIFO interrupt */
        ld3320_write_reg(ops->obj.dev, 0x2B, 0);
        ld3320_write_reg(ops->obj.dev, 0xBA, 0);
        ld3320_write_reg(ops->obj.dev, 0xBC, 0);
        ld3320_write_reg(ops->obj.dev, 0x08, 1); /* clear FIFO_DATA */
        ld3320_write_reg(ops->obj.dev, 0x08, 0); /* after clearing FIFO_DATA, write 0 again */
    }
    ld3320_hw_rst(ops);

    ld3320_init_chip(ops, LD3320_MODE_ASR);

    ld3320_addcommand_fromlist(ops);

    ld3320_asr_start(ops);
}


/**
 * @name:    ld3320_set_asr_over_callback
 * @brief:   set the callback function after LD3320 speech recognition is successful
 * @param:
 *           ops:           ld3320_t handle
 *           callback:      Callback
 * @retval:
 *           None
 */
void ld3320_set_asr_over_callback(ld3320_t ops, asr_over_callback_t callback)
{
    RT_ASSERT(ops);
    if (ops->obj.dev != RT_NULL)
    {
        ops->obj.asr_over_callback_t = callback;
    }
    else
    {
        LOG_E("fail set ld3320 asr over callback");
    }
}


#ifdef PKG_USING_LD3320_MP3
/**
 * @name:    ld3320_mp3_start
 * @brief:   start LD3320 MP3
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
void ld3320_mp3_start(ld3320_t ops)
{
    RT_ASSERT(ops);
    int fd;
    uint8_t mp3_buffer;
    uint8_t result;
    int size;
    struct stat buf;
    int ret;

    ret = stat(ops->obj.mp3_path, &buf);/* get the stat of the MP3 file */
    if (ret == 0)
    {
        ops->obj.mp3_size = buf.st_size;/* get the size of the MP3 file */
    }
    else
    {
        LOG_E("fail get mp3 file status");
    }

    fd = open(ops->obj.mp3_path, O_RDONLY);/* open mp3 file */
    if (fd >= 0)
    {
        result = ld3320_read_reg(ops->obj.dev, 0x06);

        while ((!(result & 0x08)) && (ops->obj.mp3_pos < ops->obj.mp3_size))
        {
            size = read(fd, &mp3_buffer, 1);
            if (size <= 0)
            {
                break;
            }
            ld3320_write_reg(ops->obj.dev, 0x01, mp3_buffer);
            ops->obj.mp3_pos++;
            result = ld3320_read_reg(ops->obj.dev, 0x06);/* read ld3320 status */
        }

        ld3320_write_reg(ops->obj.dev, 0xBA, 0x00);/* initialize interrupt auxiliary information */
        ld3320_write_reg(ops->obj.dev, 0x17, 0x48);/* activate DSP */
        ld3320_write_reg(ops->obj.dev, 0x33, 0x01);/* MP3 start playing */
        ld3320_write_reg(ops->obj.dev, 0x29, 0x04);/* interrupt enable */

        ld3320_write_reg(ops->obj.dev, 0x02, 0x01);/* FIFO interrupt enable */
        ld3320_write_reg(ops->obj.dev, 0x85, 0x5A);/* write 5AH when playing MP3 (change internal gain) */
    }
    close(fd);/* close mp3 file */
}
#endif


#ifdef PKG_USING_LD3320_MP3
/**
 * @name:    ld3320_mp3_run
 * @brief:   LD3320 MP3 mode operation function
 * @param:
 *           ops:   ld3320_t handle
 * @retval:
 *           None
 */
static void ld3320_mp3_run(ld3320_t ops)
{
    RT_ASSERT(ops);
    int fd;
    uint8_t result;
    uint8_t mp3_buffer;
    int size;
    uint8_t ucHighInt;
    uint8_t ucLowInt;

    fd = open(ops->obj.mp3_path, O_RDONLY);/* open mp3 file */
    if (fd >= 0)
    {
        lseek(fd, ops->obj.mp3_pos, SEEK_SET);
        ucHighInt = ld3320_read_reg(ops->obj.dev, 0x29);
        ucLowInt = ld3320_read_reg(ops->obj.dev, 0x02);
        ld3320_write_reg(ops->obj.dev, 0x29, 0);
        result = ld3320_read_reg(ops->obj.dev, 0x06);

        while ((!(result & 0x08)) && (ops->obj.mp3_pos < ops->obj.mp3_size))
        {
            size = read(fd, &mp3_buffer, 1);
            if (size <= 0)
            {
                break;
            }
            ld3320_write_reg(ops->obj.dev, 0x01, mp3_buffer);
            ops->obj.mp3_pos++;
            result = ld3320_read_reg(ops->obj.dev, 0x06);/* read ld3320 status */
        }

        ld3320_write_reg(ops->obj.dev, 0x29, ucHighInt);
        ld3320_write_reg(ops->obj.dev, 0x02, ucLowInt);
    }
    close(fd);
}
#endif


#ifdef PKG_USING_LD3320_MP3
/**
 * @name:    ld3320_set_mp3_file_path
 * @brief    LD3320 set mp3 file path function
 * @param:
 *           ops:   ld3320_t handle
 *           mp3:   file path
 * @retval:
 *           None
 * @note   This function must be called cyclically, otherwise LD3320 will not work.
 * */
void ld3320_set_mp3_file_path(ld3320_t ops, const char * mp3)
{
    rt_strncpy(ops->obj.mp3_path, mp3, rt_strlen(mp3));
    ops->obj.mp3_size = 0;
    ops->obj.mp3_pos = 0;
}
#endif


/**
 * @name:    ld3320_set_asr_over_callback
 * @brief    LD3320 heartbeat function
 * @param:
 *           ops:   ld3320_t handle
 *           mode:  mode
 * @retval:
 *           None
 * @note   This function must be called cyclically, otherwise LD3320 will not work.
 * */
void ld3320_run(ld3320_t ops, uint8_t mode)
{
    RT_ASSERT(ops);
    if (mode == LD3320_MODE_ASR)
    {
        ld3320_asr_run(ops);
    }
#ifdef PKG_USING_LD3320_MP3
    else if (mode == LD3320_MODE_MP3)
    {
        ld3320_mp3_run(ops);
    }
#endif
}
