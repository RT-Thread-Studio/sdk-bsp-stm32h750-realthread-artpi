#include <rtthread.h>
#include <ecma-globals.h>
#include <jerry_util.h>
#include <jerry_event.h>
#include <jerry_callbacks.h>
#include "mp3.h"

extern void get_songs_info(void);
static int first_time = 1;
static jerry_value_t js_message_obj = ECMA_VALUE_UNDEFINED;

jerry_value_t js_player_start_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("==>start\n");
    if (first_time)
    {
        get_songs_info();
        first_time = 0;
    }

    play_start();

    return 0;
}

jerry_value_t js_player_pause_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("==>pause\n");

    play_pause();

    return 0;
}

jerry_value_t js_player_next_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("==>next\n");
    play_next();

    return 0;
}

jerry_value_t js_player_prev_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("==>prev\n");
    play_prev();

    return 0;
}

jerry_value_t js_player_getname_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("get name :%s\n", get_song_name());

    return jerry_create_string((const jerry_char_t *)get_song_name());
}

jerry_value_t js_player_gettime_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    rt_kprintf("get time :%d\n", get_song_time());

    return jerry_create_number(get_song_time());
}

jerry_value_t js_player_setvolume_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    volume_set(args[0]/16);
    return 0;
}

jerry_value_t js_player_getvolume_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    return jerry_create_number(volume_get());
}

jerry_value_t js_player_getsource_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    return jerry_create_number(source_get());
}
jerry_value_t js_player_setsource_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    source_change();
    return 0;
}

jerry_value_t js_player_getplaymode_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    return jerry_create_number(mode_get());
}
jerry_value_t js_player_setplaymode_hdl(const jerry_value_t fun_value,
                                  const jerry_value_t this_value,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_cnt)
{
    mode_change();
    return 0;
}


static void js_message_info_free(void *native)
{
    js_message_obj = ECMA_VALUE_UNDEFINED;
    rt_kprintf("==> js_message_obj = ECMA_VALUE_UNDEFINED;\n");
}

static const jerry_object_native_info_t js_message_info =
{
    .free_cb = js_message_info_free
};

static rt_bool_t js_callback_message(jerry_value_t args)
{
    if (js_util_lock() != RT_EOK)
        return RT_FALSE;
    rt_kprintf("==> js_callback_message start\n");
    if (jerry_value_is_object(js_message_obj))
    {
        jerry_value_t msg_name = js_get_property(args, "name");
        jerry_value_t msg_data = js_get_property(args, "data");
        char str_buf[JS_STRING_BUFSZ];
        char *name = js_value_get_string(msg_name, str_buf, JS_STRING_BUFSZ);
        if (name)
        {
            js_emit_event(js_message_obj, name, &msg_data, 1);
            if (name != str_buf)
                RT_JS_FREE(name);
        }
        jerry_release_value(msg_data);
        jerry_release_value(msg_name);
    }
    jerry_release_value(args);
    rt_kprintf("==> js_callback_message end\n");
    js_util_unlock();

    return RT_TRUE;
}

static rt_bool_t js_message_send_data(const char *name, jerry_value_t data)
{
    rt_bool_t ret = RT_FALSE;

    if (js_util_lock() != RT_EOK)
        return ret;
    if (jerry_value_is_object(js_message_obj))
    {
        jerry_value_t msg = jerry_create_object();
        if (!jerry_resolve_error(msg))
        {
            js_set_string_property(msg, "name", name);
            js_set_property(msg, "data", data);

            ret = js_send_callback(js_callback_message, msg);
            if (!ret)
            {
                jerry_release_value(msg);
            }
        }
    }
    js_util_unlock();

    return ret;
}

jerry_value_t js_player_init(void)
{
    jerry_value_t js_song;

    js_song = jerry_create_object();

    REGISTER_METHOD_ALIAS(js_song, start, js_player_start_hdl);
    REGISTER_METHOD_ALIAS(js_song, pause, js_player_pause_hdl);
    REGISTER_METHOD_ALIAS(js_song, next, js_player_next_hdl);
    REGISTER_METHOD_ALIAS(js_song, prev, js_player_prev_hdl);
    REGISTER_METHOD_ALIAS(js_song, getname, js_player_getname_hdl);
    REGISTER_METHOD_ALIAS(js_song, gettime, js_player_gettime_hdl);
    REGISTER_METHOD_ALIAS(js_song, setvolume, js_player_setvolume_hdl);
    REGISTER_METHOD_ALIAS(js_song, getvolume, js_player_getvolume_hdl);
    REGISTER_METHOD_ALIAS(js_song, getsource, js_player_getsource_hdl);
    REGISTER_METHOD_ALIAS(js_song, setsource, js_player_setsource_hdl);
    REGISTER_METHOD_ALIAS(js_song, getplaymode, js_player_getplaymode_hdl);
    REGISTER_METHOD_ALIAS(js_song, setplaymode, js_player_setplaymode_hdl);

    js_message_obj = js_song;
    js_make_emitter(js_message_obj, jerry_create_undefined());
    js_set_pointer_property(js_message_obj, "_free_cb", NULL, &js_message_info);

    return js_song;
}
JS_MODULE(player, js_player_init);

int js_mp3_new(void)
{
    if (js_util_lock() == RT_EOK)
    {
        jerry_value_t value;
        value = jerry_create_number(0);
        js_message_send_data("new", value);
        jerry_release_value(value);
        js_util_unlock();
    }

    return 0;
}
