/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-03     xqyjlj       the first version
 */
#ifndef __LD3320_H__
#define __LD3320_H__

#include <rtdevice.h>
#include "ld3320_base.h"
#define LD3320_PIN_NONE                 -1

#define LD3320_MODE_ASR                 0x08
#ifdef PKG_USING_LD3320_MP3
#define LD3320_MODE_MP3                 0x80
#endif
#define LD3320_CLK_IN                   12/* LD3320 chip input frequency */

#define LD3320_PLL_11                   (uint8_t)((LD3320_CLK_IN/2.0)-1)

#ifdef PKG_USING_LD3320_MP3
#define LD3320_PLL_MP3_19               0x0f
#define LD3320_PLL_MP3_1B               0x18
#define LD3320_PLL_MP3_1D               (uint8_t)(((90.0*((LD3320_PLL_11)+1))/(LD3320_CLK_IN))-1)
#endif

#define LD3320_PLL_ASR_19               (uint8_t)(LD3320_CLK_IN*32.0/(LD3320_PLL_11+1) - 0.51)
#define LD3320_PLL_ASR_1B               0x48
#define LD3320_PLL_ASR_1D               0x1f

#define LD3320_SPEECH_END_POINT         0x22/* Adjust the end time of the voice endpoint (interval time of utterance), parameter (0x00~0xC3, unit 10MS) */
#define LD3320_SPEECH_START_TIME        0x0f/* Adjust the start time of the voice endpoint, parameters (0x00~0x30, unit 10MS) */
#define LD3320_SPEECH_END_TIME          0x3c/* Adjust the end time of the voice endpoint (interval time of utterance), parameter (0x00~0xC3, unit 10MS) */
#define LD3320_VOICE_MAX_LENGTH         0x3c/* The longest voice segment time, parameter (0x00~0xC3, unit 100MS) */
#define LD3320_NOISE_TIME               0x02/* Skip the power-on noise, parameter (0x00~0xff, unit 20MS) */
#define LD3320_MIC_VOL                  0x40/* Adjust ADC gain, parameters (0x00~0xFF, recommended 10-60) */

#define LD3320_MAX_COMMAND_LEN          20/* the max command len. unit: byte */

#define LD3320_MIX2SPVOLUME             3/* the max speaker volume */

/* ld3320 port */
struct ld3320_port
{
    int wr_pin;
    int irq_pin;
    int rst_pin;
};

typedef void (*asr_over_callback_t)(uint8_t num);/* Callback function after ld3320 asr recognition */
/* ld3320 device */
struct ld3320_obj
{
    struct ld3320_port port;
    struct rt_spi_device *dev;
    void (*asr_over_callback_t)(uint8_t num); /*callback */
#ifdef PKG_USING_LD3320_MP3
    char mp3_path[30];
    uint32_t mp3_size;
    uint32_t mp3_pos;
#endif
};

/* ld3320 list head */
struct ld3320
{
    struct ld3320_obj obj;
    rt_list_t node;
};
typedef struct ld3320 *ld3320_t;
/* ld3320 command node */
struct ld3320_command
{
    char name[LD3320_MAX_COMMAND_LEN];
    uint8_t num;
    rt_list_t list;
};
typedef struct ld3320_command *ld3320_command_t;


ld3320_t ld3320_create(char *spi_dev_name, int wr_pin, int rst_pin, int irq_pin, uint8_t mode);
void ld3320_run(ld3320_t ld3320, uint8_t mode);
void ld3320_hw_rst(ld3320_t ops);

uint8_t ld3320_asr_start(ld3320_t ops);
void ld3320_set_asr_over_callback(ld3320_t ops, asr_over_callback_t callback);
void ld3320_addcommand_tolist(ld3320_t ops, char *pass, int num);
void ld3320_addcommand_fromlist(ld3320_t ops);

#ifdef PKG_USING_LD3320_MP3
void ld3320_mp3_start(ld3320_t ops);
void ld3320_set_mp3_file_path(ld3320_t ops, const char * mp3);
#endif


#endif /* __LD3320_H__ */
