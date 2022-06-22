/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-01     Rudy Lo      The first version
 * 2022-06-20     Rbb666       Add SPI and RGB LCD Config
 */

#include <lvgl.h>

//#define DRV_DEBUG
#define LOG_TAG             "lvgl.disp"
#include <drv_log.h>

#ifdef BSP_USING_ILI9488
    #include "drv_spi_ili9488.h"
    #define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX / 5)
#else
    #include <lcd_port.h>
    #define COLOR_BUFFER  (LV_HOR_RES_MAX * LV_VER_RES_MAX / 2)
#endif

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t disp_buf;

#ifdef BSP_USING_LCD_RGB
static DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef LtdcHandle;
static rt_sem_t trans_done_semphr = RT_NULL;

static void mDMA2Dcallvack(DMA2D_HandleTypeDef *hdma2d)
{
    lv_disp_flush_ready((lv_disp_drv_t *)&disp_drv);

    rt_sem_release(trans_done_semphr);
}

static void lvgl_dma2d_config(void)
{
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_M2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
    hdma2d.Init.OutputOffset = 0;

    hdma2d.LayerCfg[1].InputOffset = 0;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
    hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputAlpha = 0;
    hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
    hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
    hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
    hdma2d.XferCpltCallback = mDMA2Dcallvack;

    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

void DMA2D_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA2D_IRQHandler(&hdma2d);

    /* exit interrupt */
    rt_interrupt_leave();
}
#endif

static void lcd_fb_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
#ifdef BSP_USING_ILI9488
    /* color_p is a buffer pointer; the buffer is provided by LVGL */
    lcd_fill_array(area->x1, area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(disp_drv);
#else
    rt_sem_take(trans_done_semphr, RT_WAITING_FOREVER);

    uint32_t OffLineSrc = LV_HOR_RES_MAX - (area->x2 - area->x1 + 1);
    uint32_t addr = (uint32_t) LtdcHandle.LayerCfg[0].FBStartAdress + 2 * (LV_HOR_RES_MAX * area->y1 + area->x1);

    DMA2D->CR &= ~(DMA2D_CR_START);
    DMA2D->CR = DMA2D_M2M;

    DMA2D->FGMAR = (uint32_t)(uint16_t *)(color_p);

    DMA2D->OMAR = (uint32_t) addr;

    DMA2D->FGOR = 0;

    DMA2D->OOR = OffLineSrc;

    DMA2D->FGPFCCR = DMA2D_OUTPUT_RGB565;
    DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;

    DMA2D->NLR = (area->y2 - area->y1 + 1) | ((area->x2 - area->x1 + 1) << 16);

    DMA2D->CR |= DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE;

    DMA2D->CR |= DMA2D_CR_START;
#endif
}

void lv_port_disp_init(void)
{
    rt_err_t result;

    static lv_color_t lv_disp_buf1[COLOR_BUFFER];

#ifdef BSP_USING_LCD_RGB
    rt_device_t lcd_device = rt_device_find("lcd");
    result = rt_device_open(lcd_device, 0);

    if (result != RT_EOK)
    {
        LOG_E("error!");
        return;
    }

    static struct rt_device_graphic_info info;
    /* get framebuffer address */
    result = rt_device_control(lcd_device, RTGRAPHIC_CTRL_GET_INFO, &info);

    if (result != RT_EOK)
    {
        LOG_E("error!");
        return;
    }

    RT_ASSERT(info.bits_per_pixel == 8 || info.bits_per_pixel == 16 ||
              info.bits_per_pixel == 24 || info.bits_per_pixel == 32);

    lvgl_dma2d_config();

    trans_done_semphr = rt_sem_create("lvgl_sem", 1, RT_IPC_FLAG_PRIO);
    if (trans_done_semphr == RT_NULL)
    {
        rt_kprintf("create transform done semphr failed.\n");
        return;
    }
#endif

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_draw_buf_init(&disp_buf, lv_disp_buf1, RT_NULL, COLOR_BUFFER);

    /*Basic initialization*/
    lv_disp_drv_init(&disp_drv);

    /*Set the resolution of the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    rt_kprintf("width: %d, height:%d\n", LV_HOR_RES_MAX, LV_VER_RES_MAX);

    /*Set a display buffer*/
    disp_drv.draw_buf = &disp_buf;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = lcd_fb_flush;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}
