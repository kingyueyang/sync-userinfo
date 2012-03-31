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

    unsigned short port = server.receiverPort;

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
     * The path /syncbasicinfo only support post mothod, 
     * it design for sync user basic information
     */
    evhttp_set_cb(http, "/syncbasicinfo", post_SBI_cb, NULL);

    /*
     * The path /syncheaderinfo only support post mothod, 
     * it design for sync user head portrait information
     */
    evhttp_set_cb(http, "/syncheaderinfo", post_SHI_cb, NULL);

    /*
     * The path /synceducationinfo only support post mothod, 
     * it design for sync user education information
     */
    evhttp_set_cb(http, "/synceducationinfo", post_SEI_cb, NULL);

    /*
     * The path /syncemploymentinfo only support post mothod, 
     * it design for sync user education information
     */
    evhttp_set_cb(http, "/syncemploymentinfo", post_SMI_cb, NULL);

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
    evhttp_send_reply(req, 401, "olo:not found this path", NULL);
    return;
}

static void
post_SMI_cb(struct evhttp_request *req, void *arg) {
    fprintf(stdout, "SMI\n");
    evhttp_send_reply(req, 200, "OK", NULL);
    return ;
}

