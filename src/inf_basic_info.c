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
    size_t evbuf_length;
    size_t proto_length;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncBasicInfo *_sync_basic_info;


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

    /* Unpack SyncBasicInfo package */
    _sync_basic_info =
        community__sync_basic_info__unpack( NULL, evbuf_length, body_buff );
    if( NULL == _sync_basic_info ) {
        evhttp_send_error( req, HTTP_BADREQUEST, 0 );
        goto CLEANUP;
    }

    proto_length =
        community__sync_basic_info__get_packed_size( _sync_basic_info );

    printf( "size:%d\t%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s,%s,%s\n",
            proto_length,
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
    fflush(stdout);

    /* Insert to ZeroMQ */
    /* If return 0 */
    evhttp_send_reply( req, 200, "OK", NULL );
    /* Else */
    /*evhttp_send_error( req, HTTP_INTERNAL, 0 );*/

CLEANUP:
    xfree(body_buff);
    if(_sync_basic_info) {
        community__sync_basic_info__free_unpacked( _sync_basic_info, NULL );
    }

    return ;
}

