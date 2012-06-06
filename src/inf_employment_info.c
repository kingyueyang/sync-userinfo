/*
 * =====================================================================================
 *
 *       Filename:  inf_employment_info.c
 *
 *    Description:  Sync user employment information call back
 *
 *        Version:  1.0
 *        Created:  03/31/2012 09:44:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_employment_info.h"

void
post_SMI_cb(struct evhttp_request *req, void *arg) {
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SMI: sync_employment_cb active");
    char *text_buf = NULL;
    char *sub_text_buf = NULL;
    size_t evbuf_length;
    size_t proto_length;
    apr_status_t push_rv;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncEmploymentInfo *_sync_employment_info;

    /* Only allow POST method */
    if(EVHTTP_REQ_POST != evhttp_request_get_command(req)) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: sync_employment_info: not http post package");
        evhttp_send_error(req, HTTP_BADMETHOD, 0);
        return ;
    }

    /* Get http buffer */
    http_buf = evhttp_request_get_input_buffer(req);
    evbuf_length = evbuffer_get_length(http_buf);
    if(evbuf_length <= 0) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: sync_employment_info: http package length less than or equal 0");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return ;
    }

    /* Remove to string */
    body_buff = (unsigned char *)xmalloc(evbuf_length + 1);
    if(NULL == body_buff) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SMI: sync_employment_info: xmalloc memory for body_buff exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return ;
    }
    size_t sz = evbuffer_remove(http_buf, body_buff, evbuf_length);

    if(sz != evbuf_length) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "sync_employment_info: http buffer length not match");
        goto CLEANUP;
    }

    /* Unpack SyncEmploymentInfo package */
    _sync_employment_info =
        community__sync_employment_info__unpack(NULL, evbuf_length, body_buff);
    if(NULL == _sync_employment_info) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: unpack SyncEmploymentInfo package exceptional");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }

    proto_length =
        community__sync_employment_info__get_packed_size(_sync_employment_info);

    text_buf = xmalloc(proto_length + 20 * 1024);
    if(NULL == text_buf) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SMI: sync_employment_info: xmalloc memory for text_buf exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        goto CLEANUP;
    }
    sub_text_buf = xmalloc(proto_length + 1500);
    if(NULL == sub_text_buf) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SMI: sync_employment_info: xmalloc memory for sub_text_buf exceptional");
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        goto CLEANUP;
    }

    if(0 == _sync_employment_info->uid) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_NOTICE,
                "SMI: uid eq zero");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }
    /* Type flag 4 */
    sprintf(text_buf, "4:%ld",_sync_employment_info->uid);
    int i;
    for(i = 0; i < _sync_employment_info->n_employments; i++) {
        snprintf (sub_text_buf, proto_length+1500, ";%d,%d,%d,%d,%s,%s",
                _sync_employment_info->employments[i]->begin_year,
                _sync_employment_info->employments[i]->begin_month,
                _sync_employment_info->employments[i]->end_year,
                _sync_employment_info->employments[i]->end_month,
                _sync_employment_info->employments[i]->company ==
		NULL ? "" : _sync_employment_info->employments[i]->company,
                _sync_employment_info->employments[i]->post ==
		NULL ? "" : _sync_employment_info->employments[i]->post
                );
        strncat(text_buf, sub_text_buf, proto_length+20*1024);
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SMI: final result >>>%s<<", text_buf);

    push_rv = apr_queue_push(queue, text_buf);
    if(APR_SUCCESS != push_rv) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_WARN,
                "SMI: push to queue failure");
	xfree(text_buf);
        /* TODO: Dual error */
    }

    evhttp_send_reply(req, 200, "OK", NULL);
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_TRACE,
            "SMI deal done");

CLEANUP:
    xfree(body_buff);
    xfree(sub_text_buf);
    /* free text_buf when after queue pop */
    if(_sync_employment_info) {
        community__sync_employment_info__free_unpacked(_sync_employment_info, NULL);
    }

    return ;
}

