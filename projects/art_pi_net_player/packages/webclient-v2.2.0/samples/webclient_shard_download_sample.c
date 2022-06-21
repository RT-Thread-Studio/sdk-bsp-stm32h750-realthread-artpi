/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-06-03    xiangxistu    the first version
 */

#include <rtthread.h>
#include <webclient.h>
#include "stdlib.h"

#define GET_LOCAL_URI                  "http://www.rt-thread.com/service/rt-thread.txt"
#define CHARACTER_LENGTH 60

/* handle function, you can store data and so on */
static int shard_download_handle(char *buffer, int length)
{
    int outindex, inindex = 0;
    int boundary;

    /* print the receive data */
    for (outindex = 0; outindex < length; outindex = outindex + inindex)
    {
        char print_buffer[CHARACTER_LENGTH + 1] = {0};
        char *point = RT_NULL;
        point = print_buffer;

        if(length - outindex > CHARACTER_LENGTH)
        {
            boundary = CHARACTER_LENGTH;
        }
        else
        {
            boundary = length - outindex;
        }

        for (inindex = 0; inindex < boundary; inindex++)
        {
            *point++ = buffer[outindex + inindex];
        }
        *point = 0;
        rt_kprintf("%04d - %04d: %s\n", outindex, outindex + boundary - 1, print_buffer);
    }

    /* release this buffer if we have handled data */
    web_free(buffer);

    return RT_EOK;
}


int webclient_shard_download_test(int argc, char **argv)
{
    struct webclient_session* session = RT_NULL;
    rt_err_t result = RT_EOK;
    char *uri = RT_NULL;
    int length = 0;
    int usage_flag = 0;
    int size = 200;


    if (argc == 1)
    {
        uri = web_strdup(GET_LOCAL_URI);
    }
    else
    {
        int index;
        for(index = 1; index < argc; index = index + 2)
        {
            if(rt_strstr(argv[index], "-u"))
            {
                uri = web_strdup(argv[index + 1]);
            }
            else if(rt_strstr(argv[index], "-l"))
            {
                size = atoi(argv[index + 1]);
            }
            else
            {
                usage_flag = 1;
                break;
            }
        }
    }

    if(usage_flag)
    {
        rt_kprintf("web_shard_test -u [URI]     - webclient HEAD and GET request test.\n");
        rt_kprintf("web_shard_test -l [SIZE]    - the length of receive buffer.\n");
        return -RT_ERROR;
    }

    if(uri == RT_NULL)
    {
        uri = web_strdup(GET_LOCAL_URI);
    }

    /* sometime, the header bufsz can set more smaller */
    session = webclient_session_create(WEBCLIENT_HEADER_BUFSZ / 4);
    if (session == RT_NULL)
    {
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* get the real data length */
    webclient_shard_head_function(session, uri, &length);

    /* register the handle function, you can handle data in the function */
    webclient_register_shard_position_function(session, shard_download_handle);

    /* the "memory size" that you can provide in the project and uri */
    result = webclient_shard_position_function(session, uri, 0, length, size);
    if(result != WEBCLIENT_OK)
    {
        rt_kprintf("web shard download, test failed!\n");
    }

    /* clear the handle function */
    webclient_register_shard_position_function(session, RT_NULL);

__exit:
    if (uri)
    {
        web_free(uri);
    }

    if (session)
    {
        webclient_close(session);
    }

    return result;
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(webclient_shard_download_test, web_shard_test, webclient head and get request test);
#endif /* FINSH_USING_MSH */
