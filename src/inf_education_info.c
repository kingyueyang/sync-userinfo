/*
 * =====================================================================================
 *
 *       Filename:  inf_educatoin_info.c
 *
 *    Description:  Sync user education information call back
 *
 *        Version:  1.0
 *        Created:  03/31/2012 07:56:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_education_info.h"

void
post_SEI_cb(struct evhttp_request *req, void *arg) {
    size_t evbuf_length;
    size_t proto_length;
    apr_status_t push_rv;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncEducationInfo *_sync_education_info;


    /* Only allow POST method */
    if(EVHTTP_REQ_POST != evhttp_request_get_command(req)) {
        evhttp_send_error(req, HTTP_BADMETHOD, 0);
        return ;
    }

    /* Get http buffer */
    http_buf = evhttp_request_get_input_buffer(req);
    evbuf_length = evbuffer_get_length(http_buf);
    if(evbuf_length <= 0) {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return ;
    }

    /* Remove to string */
    body_buff = (unsigned char *)xmalloc(evbuf_length + 1);
    if(NULL == body_buff) {
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return ;
    }
    size_t sz = evbuffer_remove(http_buf, body_buff, evbuf_length);
    assert(sz == evbuf_length);

    /* Unpack SyncEducationInfo package */
    _sync_education_info =
        community__sync_education_info__unpack(NULL, evbuf_length, body_buff);
    if(NULL == _sync_education_info) {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto CLEANUP;
    }

    proto_length =
        community__sync_education_info__get_packed_size(_sync_education_info);

    char *text_buf = xmalloc(proto_length + 1);
    char *sub_text_buf = xmalloc(proto_length + 1);
    if(NULL == text_buf || NULL == sub_text_buf) {
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        goto CLEANUP;
    }

    sprintf(text_buf, "%d",_sync_education_info->uid);
    int i;
    for(i = 0; i < _sync_education_info->n_educations; i++) {
        sprintf (sub_text_buf, ";%d,%s,%s,%d,%d",
                _sync_education_info->educations[i]->edu,
                _sync_education_info->educations[i]->school,
                _sync_education_info->educations[i]->department,
                _sync_education_info->educations[i]->class_,
                _sync_education_info->educations[i]->year
              );
        strcat(text_buf, sub_text_buf);
    }
    printf ("%s\n", text_buf);

    push_rv = apr_queue_trypush(queue, "edu");
    if(APR_SUCCESS != push_rv) {
        /* TODO: Dual error */
    }

    evhttp_send_reply(req, 200, "OK", NULL);

CLEANUP:
    xfree(body_buff);
    xfree(text_buf);
    xfree(sub_text_buf);
    if(_sync_education_info) {
        community__sync_education_info__free_unpacked(_sync_education_info, NULL);
    }

    return ;
}

