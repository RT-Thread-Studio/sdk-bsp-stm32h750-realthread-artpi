/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     liuduanfei   first version
 */

#include <rtthread.h>

#include <dirent.h>
#include <fcntl.h>

#include <rtdevice.h>
#include <string.h>
#include "mp3.h"
#include "wangyiyun.h"

#ifdef MEDIA_IO_USING_AUDIO

#include <mp3dec.h>
#define DBG_TAG "mp3"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MP3_BUF_SZ           (5 * 1024)
#define MP3_OUTPUT_BUF_SZ    (5 * 1152)
#define MP3_MAX_INPUT_SZ     (1940)
#define MP3_DEVICE_NAME      "sound0"
#define MP3_DEFAULT_VOLUME   70

static int http_mp3_time(char *url);
static int file_mp3_time(char * path);

struct mp3_decoder
{
    /* mp3 information */
    HMP3Decoder decoder;
    MP3FrameInfo frame_info;

    /* mp3 file descriptor */
    rt_size_t (*fetch_data)(void* parameter, rt_uint8_t *buffer, rt_size_t length);
    void* fetch_parameter;

    /* mp3 read session */
    rt_uint8_t *read_buffer, *read_ptr;
    rt_int16_t *output_buffer;

    /* audio device */
    rt_device_t snd_device;
};

/* play type */
enum play_type
{
    ORDER = 0,
    SINGEL,
    NONE
};

/* play status */
enum play_status
{
    RUNNING = 1,
    PAUSE
};

/* data source */
enum data_source
{
    SOURCE_HTTP = 0,
    SOURCE_FILE
};

struct player
{
    enum play_type type;
    enum play_status status;
    enum data_source source;
    rt_uint8_t volume;
};

static struct mp3_decoder _decoder = {0};
static struct player _player;

static void mp3_decoder_init()
{
    /* initialization decoder */
    _decoder.read_buffer = rt_malloc(MP3_BUF_SZ);
    if (_decoder.read_buffer == RT_NULL)
    {
        LOG_E("no memory for decoder read_buf");
        return;
    }

    _decoder.output_buffer = rt_malloc(MP3_OUTPUT_BUF_SZ);
    if (_decoder.output_buffer == RT_NULL)
    {
        LOG_E("no memory for decoder output_buf");
        rt_free(_decoder.read_buffer);
        return;
    }

    _decoder.decoder = MP3InitDecoder();

    /* find audio device */
    _decoder.snd_device = rt_device_find(MP3_DEVICE_NAME);
    if (_decoder.snd_device == RT_NULL)
    {
        rt_free(_decoder.read_buffer);
        rt_free(_decoder.output_buffer);
        LOG_E("can not fine %s device", MP3_DEVICE_NAME);
        return;
    }

    /* open audio device (play mode) */
    rt_device_open(_decoder.snd_device, RT_DEVICE_OFLAG_WRONLY);

    /* set default volume */
    struct rt_audio_caps caps;

    caps.main_type   = AUDIO_TYPE_MIXER;
    caps.sub_type    = AUDIO_MIXER_VOLUME;
    caps.udata.value = MP3_DEFAULT_VOLUME;
    rt_device_control(_decoder.snd_device, AUDIO_CTL_CONFIGURE, &caps);
}

static void mp3_decoder_detach(void)
{
    /* free audio memory */
    if (_decoder.read_buffer != RT_NULL)
    {
        rt_free(_decoder.read_buffer);
        _decoder.read_buffer = RT_NULL;
    }

    if (_decoder.output_buffer != RT_NULL)
    {
        rt_free(_decoder.output_buffer);
        _decoder.output_buffer = RT_NULL;
    }

    /* close audio device */
    if (_decoder.snd_device != RT_NULL)
    {
        rt_device_close(_decoder.snd_device);
        _decoder.snd_device = RT_NULL;
    }

    /* release mp3 decoder */
    MP3FreeDecoder(_decoder.decoder);
}

rt_mailbox_t play_mb = RT_NULL;
rt_thread_t play_thread = RT_NULL;

struct music
{
#define NMAE_LEN 64
#define PATH_LEN 256
    char mname[NMAE_LEN];
    char mpath[PATH_LEN];

    rt_uint16_t lengs;
    rt_uint16_t num;
    rt_list_t list;
};

/* music list head */
static rt_list_t music;
/* point to current music */
static struct music *music_c = RT_NULL;

static int music_num = 1;
static int c_song_id = 0;
static int song_time;

extern int js_mp3_new(void);

/* call back function */
void play_new(void)
{
    /* change UI */
    js_mp3_new();
}

/* recursive search .mp3 file */
static void music_scan(char *path)
{
    DIR *dirp = RT_NULL;
    int len = 0;

    struct dirent *d = RT_NULL;
    struct stat sta;
    struct music *new;

    dirp = opendir(path);
    chdir(path);
    while((d = readdir(dirp)) != NULL)
    {
        stat(d->d_name,&sta);
        if(!(rt_strcmp(d->d_name,".") && rt_strcmp(d->d_name,"..")))
        {
            continue;
        }

        if(S_ISDIR(sta.st_mode))
        {
            music_scan(d->d_name);
        }

        len = rt_strlen(d->d_name);

        if(!(rt_strcmp(&d->d_name[len-4],".mp3")))
        {
            new = rt_calloc(1, sizeof(struct music));
            new->num = music_num++;
            rt_strncpy(new->mname, d->d_name, len);
            if (!getcwd(new->mpath, PATH_LEN))
            {
                LOG_E("get dir failed!");
            }
            rt_sprintf(new->mpath, "%s/%s",new->mpath, new->mname);
            rt_list_insert_before(&music, &new->list);
        }
    }
    closedir(dirp);

    /* back to the root dir */
    chdir("/");
}

/* free the music information */
void music_free(void)
{
    struct music *item;
    while (rt_list_isempty(&music) == RT_FALSE)
    {
        item = rt_list_entry(music.next, struct music, list);
        rt_list_remove(&item->list);
        rt_free(item);
    }
}

/* list the music */
void music_list(void)
{
    struct music *item;
    rt_list_for_each_entry(item, &music, list)
    {
        rt_kprintf("%d, %s\n", item->num, item->mname);
    }
}

/* find next song */
static void music_next(void)
{
    if (_player.source == SOURCE_FILE)
    {
        if ((music_c == RT_NULL) || (&music == music_c->list.next))
            music_c = rt_list_entry(music.next, struct music, list);
        else
            music_c = rt_list_entry(music_c->list.next, struct music, list);
    }

    if (_player.source == SOURCE_HTTP)
    {
        if (c_song_id == NETEASE_MAX_SONG_NUM - 1)
        {
            c_song_id = 0;
        }
        else
        {
            c_song_id++;
        }
    }
}

/* find prev song */
static void music_prev(void)
{
    if (_player.source == SOURCE_FILE)
    {
        if ((music_c == RT_NULL) || (&music == music_c->list.prev))
            music_c = rt_list_entry(music.prev, struct music, list);
        else
            music_c = rt_list_entry(music_c->list.prev, struct music, list);
    }

    if (_player.source == SOURCE_HTTP)
    {
        if (c_song_id == 0)
            c_song_id = NETEASE_MAX_SONG_NUM - 1;
        else
        {
            c_song_id--;
        }
    }
}

static int mp3_decoder_run(struct mp3_decoder* decoder)
{
    struct rt_audio_caps caps = {0};

    int err;
    int samplerate = 0;
    int bytesLeft = 0;
    unsigned char* readPtr = decoder->read_buffer;

    while (1)
    {
        rt_ubase_t *value = RT_NULL;

        rt_mb_recv(play_mb, value, RT_WAITING_NO);
        if (*value == 3) /* pause */
        {
            _player.status = PAUSE;
            do
            {
                rt_mb_recv(play_mb, value, RT_WAITING_FOREVER);
            }while(*value != 2);
        }
        if (*value == 4) /* next */
        {
            music_next();
            *value = 0;
            err = -3;
            break;
        }
        if (*value == 5) /* prev */
        {
            music_prev();
            *value = 0;
            err = -4;
            break;
        }

        if(_player.status == PAUSE)
        {
            err = -5;
            break;
        }

        if (bytesLeft < MP3_MAX_INPUT_SZ)
        {
            rt_memmove(decoder->read_buffer, readPtr, bytesLeft);

            int br = decoder->fetch_data(decoder->fetch_parameter, decoder->read_buffer + bytesLeft, (MP3_BUF_SZ - bytesLeft));
            if ((br != (MP3_BUF_SZ - bytesLeft))&&(bytesLeft==0))
            {
                err = 0;
                break;
            }

            bytesLeft = bytesLeft + br;
            readPtr = decoder->read_buffer;
        }
        int offset = MP3FindSyncWord(readPtr, bytesLeft);
        if (offset < 0)
        {
            LOG_D("MP3FindSyncWord not find");
            bytesLeft=0;
            err = -1;
            continue;
        }
        else
        {
            readPtr += offset;
            bytesLeft -= offset;

            int errs = MP3Decode(decoder->decoder, &readPtr, &bytesLeft, decoder->output_buffer, 0);
            if (errs != 0)
            {
                LOG_D("MP3Decode failed %d ",errs);
                err = -2;
                break;
            }

            MP3GetLastFrameInfo(decoder->decoder, &decoder->frame_info);
            if(samplerate != decoder->frame_info.samprate)
            {
                caps.main_type               = AUDIO_TYPE_OUTPUT;
                caps.sub_type                = AUDIO_DSP_PARAM;
                caps.udata.config.samplerate = decoder->frame_info.samprate;
                caps.udata.config.channels   = decoder->frame_info.nChans;
                caps.udata.config.samplebits = decoder->frame_info.bitsPerSample;
                rt_device_control(decoder->snd_device, AUDIO_CTL_CONFIGURE, &caps);

                LOG_I("samprate      :%d", decoder->frame_info.samprate);
                LOG_I("nChans        :%d", decoder->frame_info.nChans);
                LOG_I("bitsPerSample :%d", decoder->frame_info.bitsPerSample);

                samplerate = decoder->frame_info.samprate;
            }
            rt_device_write(decoder->snd_device, 0, (const void*)decoder->output_buffer, (decoder->frame_info.outputSamps * sizeof(rt_uint16_t)));
        }
    }
    return err;
}

rt_size_t fd_fetch(void* parameter, rt_uint8_t *buffer, rt_size_t length)
{
    int fd = (int)parameter;
    return read(fd, (char*)buffer, length);
}

void file_mp3(char * path)
{
    int fd;
    int err;

    _decoder.fetch_data = fd_fetch;

    song_time = file_mp3_time(path);

    rt_kprintf("length %d.\n", song_time);

    play_new();

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open file failed");
        return;
    }

    _decoder.fetch_parameter = (void*)fd;

    err = mp3_decoder_run(&_decoder);
    if (err)
    {
        LOG_E("mp3 decoder failed, %d", err);
    }
    else
    {
        switch(_player.type)
        {
        case ORDER :
            music_next();
            break;
        case SINGEL:
            break;
        default :
            break;
        }
    }

    close(fd);
}

/* http mp3 */

#ifdef PKG_USING_WEBCLIENT
#include "webclient.h"

rt_size_t http_data_fetch(void* parameter, rt_uint8_t *buffer, rt_size_t length)
{
    int readbytes;
    struct webclient_session* session = (struct webclient_session*)parameter;
    RT_ASSERT(session != RT_NULL);

    readbytes = webclient_read(session, buffer, length);
    if (readbytes <= 0) return 0;

    return readbytes;
}

void http_mp3(char* url)
{
    int err;
    int resp_status;
    struct webclient_session* session;

    _decoder.fetch_data = http_data_fetch;

    song_time = http_mp3_time(url);

    rt_kprintf("length %d.\n", song_time);

    play_new();

    LOG_E("play songs from http: name:%s", get_song_name());

    session = webclient_session_create(2048);
    if (session == RT_NULL)
    {
        LOG_E("webclient session create failed");
        return;
    }
    /* send GET request by default header */
    if ((resp_status = webclient_get(session, url)) != 200)
    {
        rt_kprintf("webclient GET request failed, response(%d) error.\n", resp_status);
    }

    _decoder.fetch_parameter = (void *)session;

    err = mp3_decoder_run(&_decoder);
    if (err)
    {
        LOG_E("mp3 decoder failed, %d", err);
    }
    else
    {
        switch(_player.type)
        {
        case ORDER :
            music_next();
            break;
        case SINGEL:
            break;
        default :
            break;
        }
    }
    webclient_close(session);
}

void http_mp3_test(int argc, char **argv)
{
    if (argc == 1)
    {
        /* play default song */
        http_mp3("http://music.163.com/song/media/outer/url?id=540923879.mp3");
    }
    else if (argc == 2)
    {
        char url_buf[100] = {0};
        rt_sprintf(url_buf, "http://music.163.com/song/media/outer/url?id=%s.mp3", argv[1]);
        http_mp3(url_buf);
    }
    else
    {
        rt_kprintf("http_test [id]. eg: http_test 112233\n");
    }

}
MSH_CMD_EXPORT(http_mp3_test, play mp3 from http);

#endif /* PKG_USING_WEBCLIENT */

void music_start(void)
{
    if (_player.source == SOURCE_FILE)
    {
        if(music_c == RT_NULL)
        {
            music_next();
        }
        LOG_D("play %s", music_c->mpath);

        file_mp3(music_c->mpath);
    }
    if (_player.source == SOURCE_HTTP)
    {
        char url_buf[100] = {0};

        rt_kprintf("song id %d\n", c_song_id);
        rt_sprintf(url_buf, "http://music.163.com/song/media/outer/url?id=%s.mp3", songs_list->data[c_song_id].song_id);

        http_mp3(url_buf);
    }
}

void play_start(void)
{
    if(_player.status == RUNNING)
        return;
    rt_ubase_t value = 2;
    _player.status = RUNNING;
    rt_mb_send(play_mb, value);
}

void play_pause(void)
{
    if(_player.status == PAUSE)
        return;
    rt_ubase_t value = 3;
    rt_mb_send(play_mb, value);
}

void play_next(void)
{
    rt_ubase_t value = 4;
    if(_player.status == PAUSE)
    {
        music_next();
        play_start();
    }
    else
    {
        rt_mb_send(play_mb, value);
    }
}

void play_prev(void)
{
    rt_ubase_t value = 5;
    if(_player.status == PAUSE)
    {
        music_prev();
        play_start();
    }
    else
    {
        rt_mb_send(play_mb, value);
    }
}

void play_stop(void)
{
    _player.status = PAUSE;
}

char *get_song_name(void)
{
    if(_player.source == SOURCE_FILE)
    {
        return (music_c->mname);
    }
    if(_player.source == SOURCE_HTTP)
    {
        return (songs_list->data[c_song_id].song_name);
    }
    return RT_NULL;
}

int get_song_time(void)
{
    return song_time;
}

void mode_change(void)
{
    _player.type++;
    _player.type %= NONE;
    LOG_D("type %d", _player.type);
}

int mode_get(void)
{
    return _player.type;
}

int status_get(void)
{
    LOG_D("type: %d", _player.status);
    return _player.status;
}

void source_change(void)
{
    if(_player.source == SOURCE_FILE)
    {
        _player.source = SOURCE_HTTP;
        LOG_D("source: HTTP");
    }
    else
    {
        _player.source = SOURCE_FILE;
        LOG_D("source: FILE");
    }
}

int source_get(void)
{
    return _player.source;
}

void volume_set(int vol)
{
    struct rt_audio_caps caps;

    if (vol > 99) vol = 99;
    if (vol < 0) vol = 0;

    _player.volume = vol;

    /* set player volume */
    caps.main_type   = AUDIO_TYPE_MIXER;
    caps.sub_type    = AUDIO_MIXER_VOLUME;
    caps.udata.value = vol;
    rt_device_control(_decoder.snd_device, AUDIO_CTL_CONFIGURE, &caps);
}

int volume_get(void)
{
    return _player.volume;
}

void play_entry(void *parameter)
{
    rt_err_t err;
    rt_ubase_t *value = RT_NULL;

    while(1)
    {
        err = rt_mb_recv(play_mb, value, RT_WAITING_FOREVER);
        if(err != RT_EOK)
        {
            LOG_D("recv mail box err");
        }

        if (*value == 2)
        {
            while(_player.status == RUNNING)
            {
                music_start();
            }
        }
    }
}

static rt_bool_t is_player_init = RT_FALSE;
/* initialization the music player */
void player_init(void)
{
    if (is_player_init == RT_TRUE)
    {
        rt_kprintf("player already init\n");
        return;
    }

    is_player_init = RT_TRUE;

    mp3_decoder_init();

    rt_list_init(&music);

    /*
     * get song information from file system
     * make sure you have mp3 file
     */
    music_scan("/sdcard/music");

    play_mb = rt_mb_create("pl_mb", 2, RT_IPC_FLAG_FIFO);
    if(play_mb == RT_NULL)
    {
        return;
    }

    play_thread = rt_thread_create("pl_thread", play_entry, RT_NULL, 10240, 5, 10);
    if(play_thread == RT_NULL)
    {
        rt_free(play_mb);
        return;
    }
    rt_thread_startup(play_thread);

    _player.type = ORDER;
    _player.status = PAUSE;
    _player.source = SOURCE_HTTP;
    _player.volume = MP3_DEFAULT_VOLUME;

    LOG_I("music player init success.");
}

void player_deinit(void)
{
    if (is_player_init == RT_FALSE)
    {
        rt_kprintf("player not init yet \n");
        return;
    }

    is_player_init = RT_FALSE;

    music_free();

    free_songs_info();

    if (_player.status == RUNNING)
    {
        _player.status = PAUSE;
        rt_thread_mdelay(500);
    }
    mp3_decoder_detach();
}

void player_help(void)
{
    rt_kprintf("------welcome use the player------\n");
    rt_kprintf("Usage :player <cmd> <arg>.\n");
    rt_kprintf(" cmd table :\n");
    rt_kprintf("    init   : initialization the player.\n");
    rt_kprintf("    deinit : de-initialization the player.\n");
    rt_kprintf("    start  : start play the song.\n");
    rt_kprintf("    pause  : pause the song.\n");
    rt_kprintf("    prev   : play prev song.\n");
    rt_kprintf("    next   : play next song.\n");
    rt_kprintf("    source : change the data source.\n");
    rt_kprintf("    v +    : volume up.\n");
    rt_kprintf("    v -    : volume down.\n");
}

void player(int argc, char** argv)
{
    if (argc == 2)
    {
        if (rt_strcmp(argv[1], "init") == 0)
        {
            player_init();
        }
        if (rt_strcmp(argv[1], "deinit") == 0)
        {
            player_deinit();
        }

        if (rt_strcmp(argv[1], "start") == 0)
        {
            play_start();
        }
        if (rt_strcmp(argv[1], "pause") == 0)
        {
            play_pause();
        }
        if (rt_strcmp(argv[1], "next") == 0)
        {
            play_next();
        }
        if (rt_strcmp(argv[1], "prev") == 0)
        {
            play_prev();
        }
        if (rt_strcmp(argv[1], "source") == 0)
        {
            source_change();
        }
        if (rt_strcmp(argv[1], "list") == 0)
        {
            music_list();
        }
        if (rt_strcmp(argv[1], "free") == 0)
        {
            music_free();
        }
    }
    else if (argc == 3)
    {
        if((rt_strcmp(argv[1], "v") == 0))
        {
            if(rt_strcmp(argv[2], "+") == 0)
            {
                _player.volume += 2;
                volume_set(_player.volume);
            }
            if(rt_strcmp(argv[2], "-") == 0)
            {
                _player.volume -= 2;
                volume_set(_player.volume);
            }
        }
    }
    else
    {
        player_help();
    }
}
MSH_CMD_EXPORT(player, player....);

/* calculating song time */
int get_mp3_lengs(struct mp3_decoder* decoder, int size)
{
    volatile rt_int32_t bytesRead;
    volatile rt_int32_t syncWord;
    rt_uint8_t *pbuff;
    int totalTime = 0;
    int frameStart = 0;

    pbuff = (rt_uint8_t *)rt_malloc(5*1024);
    if (pbuff == RT_NULL)
    {
        return -1;
    }

    bytesRead = decoder->fetch_data(decoder->fetch_parameter, pbuff, 5*1024);
    if(bytesRead <= 0)
    {
        rt_free(pbuff);
        return -1;
    }
    if (rt_strncmp("ID3", (char *)pbuff, 3) == 0)
    {
        frameStart = 10;
    }

    syncWord = MP3FindSyncWord(pbuff, bytesRead);
    if (syncWord >= 0)
    {
        rt_int32_t p;

        MP3GetNextFrameInfo(decoder->decoder, &decoder->frame_info, pbuff+syncWord);

        if (decoder->frame_info.version == MPEG1)
        {
            p = decoder->frame_info.nChans  == 2 ? 36 : 21;
        }
        else
        {
            p = decoder->frame_info.nChans  == 2 ? 21 : 13;
        }
        p += syncWord;

        if (rt_strncmp("Xing", (char *)(pbuff + p), 4) == 0 /*|| strncmp("Info", (char *) (DecoderDef->readBuf + p), 4) == 0*/)
        {

            if ((pbuff[syncWord+43]&((unsigned char)0x01)) == 1)
            {
                rt_int32_t frames = ((rt_int32_t)pbuff[syncWord+44] << 24) | (pbuff[syncWord+45] << 16) |\
                             ((rt_int32_t)pbuff[syncWord+46] << 8) | pbuff[syncWord+47];
                totalTime = (frames-1)*261/10000;

            }
            else
            {
                totalTime =  (size-frameStart) / (decoder->frame_info.bitrate / 8); /* second */
            }
        }
        /*
         * There're two other rarely used VBR header standards: VBRI & MLLT.
         * I can't find any sample with these headers. So I just ignored them. :)
         */
        else
        {
            totalTime = (size-frameStart) / (decoder->frame_info.bitrate / 8); /* second */
        }
    }

    rt_free(pbuff);

    return totalTime;
}

int http_mp3_time(char *url)
{
    int resp_status;
    int mp3_time;
    struct webclient_session* session;

    session = webclient_session_create(2048);
    if (session == RT_NULL)
    {
        LOG_E("webclient session create failed");
        return 0;
    }

    /* send GET request by default header */
    if ((resp_status = webclient_get(session, url)) != 200)
    {
        LOG_E("webclient GET request failed, response(%d) error.\n", resp_status);
    }
    int data_size = webclient_content_length_get(session);

    _decoder.fetch_parameter = (void*)session;

    mp3_time = get_mp3_lengs(&_decoder, data_size);

    webclient_close(session);

    return mp3_time;
}

int file_mp3_time(char * path)
{
    int fd;
    int mp3_time;
    struct stat buf;

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open file failed");
        return -1;
    }

    fstat(fd, &buf);

    _decoder.fetch_parameter = (void*)fd;

    mp3_time = get_mp3_lengs(&_decoder, buf.st_size);

    close(fd);

    return mp3_time;
}

#endif /* MEDIA_IO_USING_AUDIO */
