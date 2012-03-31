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
    size_t evbuf_length;
    size_t proto_length;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncEmploymentInfo *_sync_employment_info;


    /* Only allow POST method */
    if( EVHTTP_REQ_POST != evhttp_request_get_command(req) ) {
        evhttp_send_error(req, HTTP_BADMETHOD, NULL);
        return ;
    }

    /* Get http buffer */
    http_buf = evhttp_request_get_input_buffer( req );
    evbuf_length = evbuffer_get_length( http_buf );
    if( evbuf_length <= 0 ) {
        evhttp_send_error( req, HTTP_BADREQUEST, 0 );
        return ;
    }

    /* Remove to string */
    body_buff = (unsigned char *)xmalloc( evbuf_length + 1 );
    if( NULL == body_buff ) {
        evhttp_send_error( req, HTTP_INTERNAL, 0 );
        return ;
    }
    size_t sz = evbuffer_remove( http_buf, body_buff, evbuf_length );
    assert( sz == evbuf_length );

    /* Unpack SyncEmploymentInfo package */
    _sync_employment_info =
        community__sync_employment_info__unpack(NULL, evbuf_length, body_buff);
    if(NULL == _sync_employment_info) {
        evhttp_send_error( req, HTTP_BADREQUEST, 0 );
        goto CLEANUP;
    }

    proto_length =
        community__sync_employment_info__get_packed_size( _sync_employment_info );

    printf( "uid:%d\n",_sync_employment_info->uid );
    for(int i = 0; i < 2; i++) {
        printf ( "%d,%d,%d,%d,%s,%s\n",
                _sync_employment_info->employments[i]->begin_year,
                _sync_employment_info->employments[i]->begin_month,
                _sync_employment_info->employments[i]->end_year,
                _sync_employment_info->employments[i]->end_month,
                _sync_employment_info->employments[i]->company,
                _sync_employment_info->employments[i]->post
               );
    }
    fflush(stdout);

    /* Insert to ZeroMQ */
    /* If return 0 */
    evhttp_send_reply( req, 200, "OK", NULL );
    /* Else */
    /*evhttp_send_error( req, HTTP_INTERNAL, 0 );*/

CLEANUP:
    xfree(body_buff);
    community__sync_employment_info__free_unpacked(_sync_employment_info, NULL);

    return ;
}
