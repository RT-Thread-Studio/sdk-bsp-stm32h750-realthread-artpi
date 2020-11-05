#include <rtthread.h>
#include <dfs_posix.h>
#include <rtdevice.h>
#include <string.h>
#include "board.h"

#ifdef MEDIA_IO_USING_AUDIO
#include <mp3dec.h>
#define DBG_TAG "mp3"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define MP3_AUDIO_BUF_SZ           (5 * 1024)
#define MP3_AUDIO_OUTPUT_BUF_SZ    (4 * 1153)
#define MP3_AUDIO_DEVICE_NAME      "sound0"
#define MP3_AUDIO_DEFAULT_VOLUME   60

struct mp3_decoder
{
    /* mp3 information */
    HMP3Decoder decoder;
    MP3FrameInfo frame_info;

    /* mp3 file descriptor */
	rt_size_t (*fetch_data)(void* parameter, rt_uint8_t *buffer, rt_size_t length);
	void* fetch_parameter;

    /* mp3 read session */
    rt_uint8_t *read_buffer;
    rt_int16_t *output_buffer;

	/* audio device */
	rt_device_t snd_device;
};

void mp3_decoder_init(struct mp3_decoder* decoder)
{
    RT_ASSERT(decoder != RT_NULL);

	/* initialization decoder */
    decoder->read_buffer = rt_malloc(MAINBUF_SIZE);
	if (decoder->read_buffer == RT_NULL)
    {
	    LOG_E("no memory for decoder read_buf");
        return;
    }

	decoder->output_buffer = rt_malloc(MP3_AUDIO_OUTPUT_BUF_SZ);
    if (decoder->output_buffer == RT_NULL)
    {
        LOG_E("no memory for decoder output_buf");
        rt_free(decoder->read_buffer);
        return;
    }

    decoder->decoder = MP3InitDecoder();

	/* find audio device */
	decoder->snd_device = rt_device_find(MP3_AUDIO_DEVICE_NAME);
	if (decoder->snd_device == RT_NULL)
    {
        rt_free(decoder->read_buffer);
        rt_free(decoder->output_buffer);
        LOG_E("can not fine %s device", MP3_AUDIO_DEVICE_NAME);
        return;
    }

	struct rt_audio_caps caps;

	/* set device default volume */
	caps.main_type   = AUDIO_TYPE_MIXER;
	caps.sub_type    = AUDIO_MIXER_VOLUME;
	caps.udata.value = MP3_AUDIO_DEFAULT_VOLUME;
	rt_device_control(decoder->snd_device, AUDIO_CTL_CONFIGURE, &caps);

	if (decoder->snd_device != RT_NULL)
	{
	    /* open audio device (play mode) */
		rt_device_open(decoder->snd_device, RT_DEVICE_OFLAG_WRONLY);
	}
}

void mp3_decoder_detach(struct mp3_decoder* decoder)
{
    RT_ASSERT(decoder != RT_NULL);

    /* free audio memory */
    if (decoder->read_buffer != RT_NULL)
    {
        rt_free(decoder->read_buffer);
        decoder->read_buffer = RT_NULL;
    }

    if (decoder->output_buffer != RT_NULL)
    {
        rt_free(decoder->output_buffer);
        decoder->output_buffer = RT_NULL;
    }

	/* close audio device */
	if (decoder->snd_device != RT_NULL)
		rt_device_close(decoder->snd_device);

	/* release mp3 decoder */
    MP3FreeDecoder(decoder->decoder);
}

struct mp3_decoder* mp3_decoder_create()
{
    struct mp3_decoder* decoder;

	/* allocate object */
    decoder = (struct mp3_decoder*) rt_malloc (sizeof(struct mp3_decoder));
    if (decoder != RT_NULL)
    {
        mp3_decoder_init(decoder);
    }

    return decoder;
}

void mp3_decoder_delete(struct mp3_decoder* decoder)
{
    RT_ASSERT(decoder != RT_NULL);

	/* detach mp3 decoder object */
	mp3_decoder_detach(decoder);

	/* release this object */
    rt_free(decoder);
    decoder = RT_NULL;
}

int mp3_decoder_run(struct mp3_decoder* decoder)
{
    struct rt_audio_caps caps = {0};

    int err;
    int samplerate = 0;
    int bytesLeft = 0;
    unsigned char* readPtr = decoder->read_buffer;

    while (1)
    {
        if (bytesLeft < MAINBUF_SIZE)
        {
            rt_memmove(decoder->read_buffer, readPtr, bytesLeft);

            int br = decoder->fetch_data(decoder->fetch_parameter, decoder->read_buffer + bytesLeft, (MAINBUF_SIZE - bytesLeft));
            if ((br != (MAINBUF_SIZE - bytesLeft))&&(bytesLeft==0))
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
                caps.udata.config.samplebits = 16;
                rt_device_control(decoder->snd_device, AUDIO_CTL_CONFIGURE, &caps);

                samplerate = decoder->frame_info.samprate;
            }
            rt_device_write(decoder->snd_device, 0, (const void*)decoder->output_buffer, (decoder->frame_info.outputSamps * sizeof(rt_uint16_t)));
        }
    }
    return err;
}

#include <finsh.h>
rt_size_t fd_fetch(void* parameter, rt_uint8_t *buffer, rt_size_t length)
{
	int fd = (int)parameter;

	return read(fd, (char*)buffer, length);
}

void mp3(void)
{
	int fd;
	int err;
	struct mp3_decoder* decoder;
	
	fd = open("/sdcard/music/RT-Thread.mp3", O_RDONLY);
	if (fd < 0)
	{
        LOG_E("open file failed");
    }
    decoder = mp3_decoder_create();
    if (decoder != RT_NULL)
    {
        decoder->fetch_data = fd_fetch;
        decoder->fetch_parameter = (void*)fd;

        err = mp3_decoder_run(decoder);
        if (err)
        {
            LOG_E("mp3 decoder failed, %d", err);
        }

        /* delete decoder object */
        mp3_decoder_delete(decoder);
    }
    else
    {
        LOG_E("mp3 decoder create failed");
    }
    close(fd);
}
MSH_CMD_EXPORT(mp3, mp3 decode test);

#endif /* MEDIA_IO_USING_AUDIO */

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
	struct mp3_decoder* decoder;

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

    decoder = mp3_decoder_create();
    if (decoder != RT_NULL)
    {
        decoder->fetch_data = http_data_fetch;
        decoder->fetch_parameter = (void *)session;

        err = mp3_decoder_run(decoder);
        if (err)
        {
            LOG_E("mp3 decoder failed, %d", err);
        }

        /* delete decoder object */
        mp3_decoder_delete(decoder);
    }
    else
    {
        LOG_D("mp3 decoder create failed");
    }
    webclient_close(session);
}

void http_test(int argc, char **argv)
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
MSH_CMD_EXPORT(http_test, http test);

#endif /* PKG_USING_WEBCLIENT */
