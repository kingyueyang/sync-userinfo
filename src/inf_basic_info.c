/*
 * =====================================================================================
 *
 *       Filename:  inf_basic_info.c
 *
 *    Description:  Sync user basic information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 05:53:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_basic_info.h"

void
post_SBI_cb(struct evhttp_request *req, void *arg) {
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SBI: sync_basic_cb active");
    size_t evbuf_length;
    size_t proto_length;
    apr_status_t push_rv;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncBasicInfo *_sync_basic_info;


    /* Only allow POST method */
    if(EVHTTP_REQ_POST != evhttp_request_get_command(req)) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SBI: sync_basic_info: not http post package");
        evhttp_send_error(req, HTTP_BADMETHOD, NULL);
        return ;
    }

    /* Get http buffer */
    http_buf = evhttp_request_get_input_buffer(req);
    evbuf_length = evbuffer_get_length(http_buf);
    if(evbuf_length <= 0) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SBI: sync_basic_info: http package length less than or equal 0");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return ;
    }

    /* Remove to string */
    body_buff = (unsigned char *)xmalloc(evbuf_length + 1);
    if(NULL == body_buff) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SBI: sync_basic_info: xmalloc memory for body_buff exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return ;
    }
    size_t sz = evbuffer_remove(http_buf, body_buff, evbuf_length);
    assert(sz == evbuf_length);

    write(2, body_buff, evbuf_length);

    /* Unpack SyncBasicInfo package */
    _sync_basic_info =
        community__sync_basic_info__unpack(NULL, evbuf_length, body_buff);
    if(NULL == _sync_basic_info) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SBI: unpack SyncBasicInfo package exceptional");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }

    proto_length =
        community__sync_basic_info__get_packed_size(_sync_basic_info);

    char *text_buf = xmalloc(proto_length + 15);
    if(NULL == text_buf) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SBI: sync_basic_info: xmalloc memory for text_buf exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        goto CLEANUP;
    }

    /* Type flag 1 */
    sprintf( text_buf, "1:%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s,%s,%s",
            _sync_basic_info->uid,
            _sync_basic_info->birth_year,
            _sync_basic_info->birth_month,
            _sync_basic_info->birth_day,
            _sync_basic_info->constellation,
            _sync_basic_info->blood_types,
            _sync_basic_info->sex,
            _sync_basic_info->home_nation,
            _sync_basic_info->home_pro,
            _sync_basic_info->home_city,
            _sync_basic_info->now_nation,
            _sync_basic_info->now_pro,
            _sync_basic_info->now_city
           );
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SBI: final result >>>%s<<", text_buf);

    push_rv = apr_queue_push(queue, text_buf);
    if(APR_SUCCESS != push_rv) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "push to queue failure");
        /* TODO: Dual error */
    }

    evhttp_send_reply(req, 200, "OK", NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SBI deal done");

CLEANUP:
    xfree(body_buff);
    /* free text_buf when after queue pop */

    if(_sync_basic_info) {
        community__sync_basic_info__free_unpacked(_sync_basic_info, NULL);
    }

    return ;
}

