/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-03-22     Murphy       the first version
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtthread.h>
#include <finsh.h>

#include "webclient.h"
#include <fal.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME          "http_ota"
#ifdef OTA_DOWNLOADER_DEBUG
#define DBG_LEVEL                 DBG_LOG
#else
#define DBG_LEVEL                 DBG_INFO
#endif
#define DBG_COLOR
#include <rtdbg.h>

#ifdef PKG_USING_HTTP_OTA

#define HTTP_OTA_BUFF_LEN         1024
#define GET_HEADER_BUFSZ          512
#define GET_RESP_BUFSZ            512
#define HTTP_OTA_DL_DELAY         (10 * RT_TICK_PER_SECOND)

#define HTTP_OTA_URL              PKG_HTTP_OTA_URL

/* the address offset of download partition */
#ifndef PKG_USING_FAL
#error "Please enable and confirgure FAL part."
#endif /* PKG_USING_FAL */
const struct fal_partition * dl_part = RT_NULL;
static int begin_offset = 0;
static int file_size = 0;

static void print_progress(size_t cur_size, size_t total_size)
{
    static unsigned char progress_sign[100 + 1];
    uint8_t i, per = cur_size * 100 / total_size;

    if (per > 100)
    {
        per = 100;
    }

    for (i = 0; i < 100; i++)
    {
        if (i < per)
        {
            progress_sign[i] = '=';
        }
        else if (per == i)
        {
            progress_sign[i] = '>';
        }
        else
        {
            progress_sign[i] = ' ';
        }
    }

    progress_sign[sizeof(progress_sign) - 1] = '\0';

    LOG_I("Download: [%s] %03d%%\033[1A", progress_sign, per);
}

/* handle function, you can store data and so on */
static int http_ota_shard_download_handle(char *buffer, int length)
{
    int ret = RT_EOK;
    /* Write the data to the corresponding partition address */
    if (fal_partition_write(dl_part, begin_offset, buffer, length) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) write data error!", dl_part->name);
        ret = -RT_ERROR;
        goto __exit;
    }
    begin_offset += length;

    print_progress(begin_offset, file_size);

__exit:
    rt_free(buffer);
    return ret;
}

static int http_ota_fw_download(const char* uri)
{
    int ret = RT_EOK;
    struct webclient_session* session = RT_NULL;

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (!session)
    {
        LOG_E("open uri failed.");
        ret = -RT_ERROR;
        goto __exit;
    }

    /* get the real data length */
    webclient_shard_head_function(session, uri, &file_size);

    if (file_size == 0)
    {
        LOG_E("Request file size is 0!");
        ret = -RT_ERROR;
        goto __exit;
    }
    else if (file_size < 0)
    {
        LOG_E("webclient GET request type is chunked.");
        ret = -RT_ERROR;
        goto __exit;
    }
    LOG_I("OTA file size is (%d)", file_size);
    LOG_I("\033[1A");

    /* Get download partition information and erase download partition data */
    if ((dl_part = fal_partition_find("download")) == RT_NULL)
    {
        LOG_E("Firmware download failed! Partition (%s) find error!", "download");
        ret = -RT_ERROR;
        goto __exit;
    }

    LOG_I("Start erase flash (%s) partition!", dl_part->name);

    if (fal_partition_erase(dl_part, 0, file_size) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) erase error!", dl_part->name);
        ret = -RT_ERROR;
        goto __exit;
    }
    LOG_I("Erase flash (%s) partition success!", dl_part->name);

    /* register the handle function, you can handle data in the function */
    webclient_register_shard_position_function(session, http_ota_shard_download_handle);

    /* the "memory size" that you can provide in the project and uri */
    ret = webclient_shard_position_function(session, uri, begin_offset, file_size, HTTP_OTA_BUFF_LEN);

    /* clear the handle function */
    webclient_register_shard_position_function(session, RT_NULL);

    if (ret == RT_EOK)
    {
        if (session != RT_NULL)
        {
            webclient_close(session);
            session = RT_NULL;
        }

        LOG_I("\033[0B");
        LOG_I("Download firmware to flash success.");
        LOG_I("System now will restart...");

        rt_thread_delay(rt_tick_from_millisecond(5));

        /* Reset the device, Start new firmware */
        extern void rt_hw_cpu_reset(void);
        rt_hw_cpu_reset();
    }
    else
    {
        LOG_E("Download firmware failed.");
    }

__exit:
    if (session != RT_NULL)
        webclient_close(session);
    begin_offset = 0;

    return ret;
}

void http_ota(uint8_t argc, char **argv)
{
    if (argc < 2)
    {
        rt_kprintf("using uri: " HTTP_OTA_URL "\n");
        http_ota_fw_download(HTTP_OTA_URL);
    }
    else
    {
        http_ota_fw_download(argv[1]);
    }
}
/**
 * msh />http_ota [url]
*/
MSH_CMD_EXPORT(http_ota, Use HTTP to download the firmware);

#endif /* PKG_USING_HTTP_OTA */
