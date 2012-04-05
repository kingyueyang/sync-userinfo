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
    size_t evbuf_length;
    size_t proto_length;
    unsigned char *body_buff = NULL;
    struct evbuffer *http_buf;
    Community__SyncHeaderInfo *_sync_header_info;


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

    /* Unpack SyncHeaderInfo package */
    _sync_header_info =
        community__sync_header_info__unpack( NULL, evbuf_length, body_buff );
    if( NULL == _sync_header_info ) {
        evhttp_send_error( req, HTTP_BADREQUEST, 0 );
        goto CLEANUP;
    }

    proto_length =
        community__sync_header_info__get_packed_size( _sync_header_info );

    char *text_buf = xmalloc( proto_length + 1 );
    if(NULL == text_buf) {
        evhttp_send_error( req, HTTP_INTERNAL, 0 );
        goto CLEANUP;
    }

    sprintf( text_buf, "%d,%d",
            _sync_header_info->uid,
            _sync_header_info->header
           );
    printf ( "%s\n", text_buf );

    /* Insert to Queue */
    /*add_queue_item( queue, text_buf, NULL, proto_length );*/

    evhttp_send_reply( req, 200, "OK", NULL );

CLEANUP:
    xfree(body_buff);
    xfree(text_buf);
    if(_sync_header_info) {
        community__sync_header_info__free_unpacked(_sync_header_info, NULL);
    }

    return ;
}

