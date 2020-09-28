#include "common.h"
#include <webnet.h>
#include <wn_module.h>
#include "core_cgi_data.h"

static void cgi_basic_info(struct webnet_session* session)
{
    char *json_data = core_cgi_serialize_basic_info();

    const char* mimetype;
    mimetype = mime_get_type(".txt");
    session->request->result_code = 200;
    webnet_session_set_header(session, mimetype, 200, "Ok", rt_strlen(json_data));
    webnet_session_write(session, (const rt_uint8_t *)json_data, rt_strlen(json_data));

    rt_free(json_data);
}

static void cgi_plugins_info(struct webnet_session* session)
{
    char *json_data = core_cgi_serialize_plugins_info();

    const char* mimetype;
    mimetype = mime_get_type(".txt");
    session->request->result_code = 200;
    webnet_session_set_header(session, mimetype, 200, "Ok", rt_strlen(json_data));
    webnet_session_write(session, (const rt_uint8_t *)json_data, rt_strlen(json_data));

    rt_free(json_data);
}

struct core_cgi_item
{
    const char *name;
    void (*handler)(struct webnet_session *session);
};

static const struct core_cgi_item items[] = {
    {"basic_info", cgi_basic_info},
    {"plugins_info", cgi_plugins_info}
};

static int core_cgi_init(void)
{
    int cnt =  sizeof(items) / sizeof(items[0]);
    for (int i = 0; i < cnt; i++)
    {
        webnet_cgi_register(items[i].name, items[i].handler);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(core_cgi_init);
