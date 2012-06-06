/*
 * =====================================================================================
 *
 *       Filename:  inf_header_info.c
 *
 *    Description:  Sync user head portrait information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 06:34:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_header_info.h"

void
post_SHI_cb(struct evhttp_request *req, void *arg) {
    char *text_buf = NULL;
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SHI: sync_header_cb active");
    size_t evbuf_length;
    size_t proto_length;
    apr_status_t push_rv;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncHeaderInfo *_sync_header_info;

    /* Only allow POST method */
    if(EVHTTP_REQ_POST != evhttp_request_get_command(req)) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: sync_header_info: not http post package");
        evhttp_send_error(req, HTTP_BADMETHOD, NULL);
        return ;
    }

    /* Get http buffer */
    http_buf = evhttp_request_get_input_buffer(req);
    evbuf_length = evbuffer_get_length(http_buf);
    if(evbuf_length <= 0) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: sync_header_info: http package length less than or equal 0");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return ;
    }

    /* Remove to string */
    body_buff = (unsigned char *)xmalloc(evbuf_length + 1);
    if(NULL == body_buff) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SMI: sync_header_info: xmalloc memory for body_buff exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return ;
    }
    size_t sz = evbuffer_remove(http_buf, body_buff, evbuf_length);

    if(sz != evbuf_length) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "sync_header_info: http buffer length not match");
        goto CLEANUP;
    }

    /* Unpack SyncHeaderInfo package */
    _sync_header_info =
        community__sync_header_info__unpack(NULL, evbuf_length, body_buff);
    if(NULL == _sync_header_info) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SHI: unpack SyncHeaderInfo package exceptional");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }

    proto_length =
        community__sync_header_info__get_packed_size(_sync_header_info);

    text_buf = xmalloc(proto_length + 1024);
    if(NULL == text_buf) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SHI: sync_header_info: xmalloc memory for text_buf exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        goto CLEANUP;
    }

    if(0 == _sync_header_info->uid) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SHI: uid eq zero");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }
    /* Type flag 2 */
    sprintf(text_buf, "2:%ld,%d",
            _sync_header_info->uid,
            _sync_header_info->header
          );
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SHI: final result >>>%s<<", text_buf);

    push_rv = apr_queue_push(queue, text_buf);
    if(APR_SUCCESS != push_rv) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SHI: push to queue failure");
	xfree(text_buf);
        /* TODO: Dual error */
    }

    evhttp_send_reply(req, 200, "OK", NULL);

CLEANUP:
    xfree(body_buff);
    /* free text_buf when after queue pop */
    if(_sync_header_info) {
        community__sync_header_info__free_unpacked(_sync_header_info, NULL);
    }

    return ;
}

