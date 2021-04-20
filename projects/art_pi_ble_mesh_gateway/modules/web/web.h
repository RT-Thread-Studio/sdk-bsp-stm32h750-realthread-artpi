#ifndef __WEB_H
#define __WEB_H
#include <webnet.h>
#include <cJSON.h>
#include <wn_module.h>

#define cgi_head()                       \
    ;                                    \
    const char *mimetype;                \
    struct webnet_request *request;      \
    static char *body = NULL;            \
    request = session->request;          \
    mimetype = mime_get_type(".html");   \
    session->request->result_code = 200; \
    webnet_session_set_header(session, mimetype, 200, "Ok", -1);


int web_init(void);

#endif /* __WEB_H */
