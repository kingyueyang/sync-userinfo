/*
 * =====================================================================================
 *
 *       Filename:  test_user_header_info.c
 *
 *    Description:  send user header info to sync user system
 *
 *        Version:  1.0
 *        Created:  03/31/2012 10:19:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "test_user_header_info.h"

int
main( int argc, char *argv[] ) {
    int64_t uid = 1057346344;
    int32_t header = 1;

    Community__SyncHeaderInfo sync_header_info = COMMUNITY__SYNC_HEADER_INFO__INIT;

    sync_header_info.uid = uid;
    sync_header_info.header = header;

    size_t sync_header_info_size =
        community__sync_header_info__get_packed_size(&sync_header_info);
    char *sync_header_info_buff = xmalloc(sync_header_info_size);

    size_t rc_size = 
        community__sync_header_info__pack(&sync_header_info, sync_header_info_buff);
    assert(sync_header_info_size == rc_size);

    char *server_url = "http://127.0.0.1:8080/syncheaderinfo";
    http_post_pb(sync_header_info_buff, rc_size, server_url);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

