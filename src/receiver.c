/*
 * =====================================================================================
 *
 *       Filename:  receiver.c
 *
 *    Description:  receiver sync information
 *
 *        Version:  1.0
 *        Created:  03/30/2012 04:19:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "receiver.h"

extern struct syncServer server;

int
receiver( void ) {
    printf( "receiver\n" );
    printf ( "%s:%d\n", server.receiverIP, server.receiverPort );

    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;

    unsigned short port = server.receiverIP;

    /* As you konw */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return (1);

    /* Create a new base evObject */
    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Couldn't create an event_base: exiting\n");
        return 1;
    }

    /* Create a new evhttp object to handle requests. */
    http = evhttp_new(base);
    if (!http) {
        fprintf(stderr, "couldn't create evhttp. Exiting.\n");
        return 1;
    }

    /*
     * The path /post/syncbasicinfo only support post mothod, 
     * it design for sync user basic information
     */
    evhttp_set_cb(http, "/get", get_config_cb, NULL);

    /*
     * The path /post support post method
     * When sharding node need to be grant, it can post request
     */
    evhttp_set_cb(http, "/post", post_grant_cb, NULL);

    /* We want to accept arbitrary requests, so we need to set a "generic"
     * cb.  We can also add callbacks for specific paths. */
    evhttp_set_gencb(http, other_cb, NULL);

    /* Now we tell the evhttp what port to listen on */
    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);

    if (!handle) {
        fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
                (int)port);
        return 1;
    }

    event_base_dispatch(base);

    return EXIT_SUCCESS;
}

static void
other_cb(struct evhttp_request *req, void *arg) {
    printf("Other Path\n");
    evhttp_send_reply(req, 400, "olo", NULL);
    return;
}

static void
get_config_cb(struct evhttp_request *req, void *arg) {
    struct evbuffer *buf = evbuffer_new();

    if (EVHTTP_REQ_GET != evhttp_request_get_command(req)) {
        evhttp_send_reply(req, 500, "not support this method", NULL);
        /*log it*/
        printf("/get Not Support This Method\n");
        evbuffer_free(buf);
        return;
    }

    static char test_char[] = 
        "ok, you get this message";
    char *msg = NULL;
    /*gen_data(&msg);*/
    printf("out: %s\n", msg);

    evbuffer_add(buf, msg, strlen(msg));
    evhttp_send_reply(req, 200, "OK", buf);
    printf("test get\n");
    printf("====================\n");

    evbuffer_free(buf);
    free(msg);
    msg = NULL;
    return ;
}

static void
post_grant_cb(struct evhttp_request *req, void *arg) {
    struct evbuffer *buf;
    char *buffer = NULL;

    printf("POST Request\n");

    if (EVHTTP_REQ_POST != evhttp_request_get_command(req)) {
        evhttp_send_reply(req, 500, "not support this method", NULL);
        /*log it*/
        printf("/post not support this method\n");
        return;
    }

    buf = evhttp_request_get_input_buffer(req);
    size_t sz = evbuffer_get_length(buf);

    buffer = malloc(sz);
    if (NULL == buffer) {
        evhttp_send_reply(req, 500, "alloc memroy error", NULL);
        return ;
    }

    /* Init temp buffer */
    memset(buffer, 0, sz);
    while (evbuffer_get_length(buf)) {
        int n;
        n = evbuffer_remove(buf, buffer, sz);
    }
    /*parser(buffer, sz);*/

    free(buffer);
    buffer = NULL;

    evhttp_send_reply(req, 200, "OK", NULL);
    return ;
}
