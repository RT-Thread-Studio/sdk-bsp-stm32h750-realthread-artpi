#include <rtthread.h>
#include <webclient.h>
#include <string.h>
#include <stdio.h>
#include <iconv.h>
#include "wangyiyun.h"

#define GET_HEADER_BUFSZ               1024
#define GET_TOP100_RESP_BUFSZ          (1024 * 300)
#define GET_DETAIL_RESP_BUFSZ          (1024 * 20)

#define NETEASE_TOP100_URI              "https://music.163.com/#/discover/toplist"
#define NETEASE_SONG_DOWNLOAD_URL       "http://music.163.com/song/media/outer/url?id=%d.mp3"
#define NETEASE_DETAIL_DOWNLOAD_URL     "https://api.imjad.cn/cloudmusic/?type=detail&id=%d"
#define NETEASE_SONG_PATH               "./music/%s.mp3"
#define NETEASE_PIC_PATH                "./music/%s.jpg"

#define DBG_TAG "wangyiyun"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static int netease_get_response(char *url, unsigned char *buf, size_t buf_len)
{
    struct webclient_session* session = RT_NULL;
    int bytes_read, resp_status;
    int content_length = -1;
    uint32_t recv_sum = 0;

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (session == RT_NULL)
    {
        rt_kprintf("no memory!\n");
        goto __exit;
    }

    /* send GET request by default header */
    if ((resp_status = webclient_get(session, url)) != 200)
    {
        LOG_D("webclient GET request failed, response(%d) error.", resp_status);
        goto __exit;
    }

    content_length = webclient_content_length_get(session);
    if (content_length < 0)
    {
        LOG_D("webclient GET request type is chunked.\n");
        do
        {
            bytes_read = webclient_read(session, &buf[recv_sum], buf_len);
            recv_sum += bytes_read;
            if (bytes_read <= 0)
            {
                LOG_D("recv end! %d\n", recv_sum);
                break;
            }
        } while (1);
    }
    else
    {
        int content_pos = 0;

        do
        {
            bytes_read = webclient_read(session, &buf[recv_sum], 
                    content_length - content_pos > buf_len ?
                            buf_len : content_length - content_pos);
            recv_sum += bytes_read;
            if (bytes_read <= 0)
            {
                break;
            }

            content_pos += bytes_read;
        } while (content_pos < content_length);
    }

__exit:
    if (session)
    {
        webclient_close(session);
    }

    return recv_sum;

}

static int netease_get_list(netease_song_list_t * list)
{
    unsigned char *buffer = NULL;
    char *start_ptr = NULL;
    char *end_ptr = NULL;
    char *song_ptr = NULL;
    int song_id = 0;
    char song_name_buf[128] = {0};
    char song_id_buf[15] = {0};
    int i = 0;
    int recv_sum = 0;
    int ret = 0;

    list->song_data_num = 0;

    buffer = (unsigned char *)web_malloc(GET_TOP100_RESP_BUFSZ);
    if (buffer == RT_NULL)
    {
        LOG_D("no memory for receive buffer.\n");
        goto __exit;
    }

    recv_sum = netease_get_response(NETEASE_TOP100_URI, buffer, GET_TOP100_RESP_BUFSZ);
    if (recv_sum > 0 )
    {
        start_ptr = strstr((const char *)buffer, "加载中...");
        end_ptr = strstr((const char *)buffer, "song-list-pre-data");
        if(start_ptr == NULL || end_ptr == NULL)
        {
           goto __exit;
        }

        song_ptr = start_ptr;

        for(i = 0; i < NETEASE_MAX_SONG_NUM; i++)
        {
            song_ptr = strstr((const char *)song_ptr, "<a href=");
            if(song_ptr > start_ptr && song_ptr < end_ptr)
            {
                ret = sscanf(song_ptr, "%*[^a href=\"/]a href=\"/song?id=%d\">%[^</a>]", &song_id, song_name_buf);
                if(ret != 0)
                {
                    list->song_data_num ++;
                    sprintf(song_id_buf, "%d", song_id);
                    list->data[i].song_id = rt_strdup(song_id_buf);
                    list->data[i].song_name = rt_strdup(song_name_buf);
                }
                else
                {
                    LOG_D("don`t find song id\n");
                }
            }
            else
            {
                goto __exit;
            }
            song_ptr = song_ptr + 5;
        }
    }

__exit:
    if(buffer)
    {
        web_free(buffer);
    }

    return list->song_data_num;
}

netease_song_list_t *netease_new_list()
{
    netease_song_list_t *netease_list;
    netease_list = rt_malloc(sizeof(netease_song_list_t));
    if(netease_list != NULL)
    {
        memset(netease_list, 0, sizeof(netease_song_list_t));
    }

    return netease_list;
}

int netease_free_list(netease_song_list_t * list)
{
    int i = 0;

    if(list == NULL)
    {
        goto __exit;
    }

    for(i = 0; i < list->song_data_num; i++)
    {
        if(list->data[i].song_name)
        {
            rt_free(list->data[i].song_name);
        }
        if(list->data[i].song_id)
        {
            rt_free(list->data[i].song_id);
        }
    }

    rt_free(list);

__exit:
    return RT_EOK;
}

netease_song_list_t *songs_list = RT_NULL;

void get_songs_info(void)
{
    if (songs_list != RT_NULL)
    {
        netease_free_list(songs_list);
        songs_list = RT_NULL;
    }
    songs_list = netease_new_list();
    netease_get_list(songs_list);
}

void free_songs_info(void)
{
    if (songs_list != RT_NULL)
    {
        netease_free_list(songs_list);
        songs_list = RT_NULL;
    }
}

void list_songs_info(void)
{
    for(int i=0; i < NETEASE_MAX_SONG_NUM; i++)
    {
        rt_kprintf("id %s ", songs_list->data[i].song_id);
        rt_kprintf("name %s \n", songs_list->data[i].song_name);
    }
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT(get_songs_info, ......);
MSH_CMD_EXPORT(list_songs_info, ......);
#endif /* FINSH_USING_MSH */
