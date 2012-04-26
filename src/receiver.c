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

void *
receiver(void *args) {
    sleep(1);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: receiver module starting with IP:%s, Posr:%d",
            server.receiverIP, server.receiverPort);

    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;

    /* As you konw */
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_ERROR,
                "receiver: signal PIPE bind failure");
        return ((void *)-1);
    }

    /* Create a new base evObject */
    base = event_base_new();
    if(!base) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_ERROR,
                "receiver: couldn't create an event_base: exiting");
        return ((void *)-2);
    }

    /* Create a new evhttp object to handle requests. */
    http = evhttp_new(base);
    if(!http) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_ERROR,
                "receiver: couldn't create evhttp. Exiting");
        return ((void *)-3);
    }

    /*
     * The path /syncbasicinfo only support post mothod, 
     * it design for sync user basic information
     */
    evhttp_set_cb(http, "/syncbasicinfo", post_SBI_cb, NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: sync_basic_info path has been set");

    /*
     * The path /syncheaderinfo only support post mothod, 
     * it design for sync user head portrait information
     */
    evhttp_set_cb(http, "/syncheaderinfo", post_SHI_cb, NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: sync_header_info path has been set");

    /*
     * The path /synceducationinfo only support post mothod, 
     * it design for sync user education information
     */
    evhttp_set_cb(http, "/synceducationinfo", post_SEI_cb, NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: sync_education_info path has been set");

    /*
     * The path /syncemploymentinfo only support post mothod, 
     * it design for sync user education information
     */
    evhttp_set_cb(http, "/syncemploymentinfo", post_SMI_cb, NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: sync_employment path has been set");

    /* We want to accept arbitrary requests, so we need to set a "generic"
     * cb.  We can also add callbacks for specific paths. */
    evhttp_set_gencb(http, other_cb, NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "receiver: other path has been set");

    /* Now we tell the evhttp what port to listen on */
    handle = evhttp_bind_socket_with_handle(
            http, server.receiverIP, server.receiverPort);

    if(!handle) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_ERROR,
                "receiver: couldn't bind to ip:%s, port %u. Exiting",
                server.receiverIP, (unsigned int)server.receiverPort);
        return ((void *)-4);
    }

    log4c_category_log(
            log_handler, LOG4C_PRIORITY_INFO,
            "receiver: recevier start up");

    event_base_dispatch(base);

    return ((void *)0);
}

static void
other_cb(struct evhttp_request *req, void *arg) {
    printf("Other Path\n");
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_NOTICE,
            "receiver: some data arrive unkonw path, remote ip, port");
    /*TODO:log remote ip, port*/
    evhttp_send_reply(req, 401, "olo:not found this path", NULL);

    return;
}

