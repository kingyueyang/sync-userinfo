/*
 * =====================================================================================
 *
 *       Filename:  test_user_employment_info.c
 *
 *    Description:  send user employment info to sync user system
 *
 *        Version:  1.0
 *        Created:  03/31/2012 09:12:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "test_user_employment_info.h"

int
main( int argc, char *argv[] ) {
    size_t sync_employment_info_size;
    int64_t uid = 407902377;
    int32_t begin_year[] = {2003, 2006};
    int32_t begin_month[] = {02, 04};
    int32_t end_year[] = {2006, 2009};
    int32_t end_month[] = {03, 05};
    char company[][32] = {"Amazon", "Facebook"};
    char post[][32] = {"Senior Engineer", "PM"};

    /* Education packages */
    Community__Employment **employments;
    employments = xmalloc(sizeof(Community__Employment) * 2);

    for(int i = 0; i < 2; i++) {
        employments[i] = xmalloc( sizeof(Community__Employment) );
        community__employment__init( employments[i] );
        employments[i]->begin_year = begin_year[i];
        employments[i]->begin_month = begin_month[i];
        employments[i]->end_year = end_year[i];
        employments[i]->end_month = end_month[i];
        employments[i]->company = company[i];
        employments[i]->post = post[i];
    }

    Community__SyncEmploymentInfo sync_employment_info =
        COMMUNITY__SYNC_EMPLOYMENT_INFO__INIT;
    sync_employment_info.uid = uid;
    sync_employment_info.n_employments = 2;
    sync_employment_info.employments = employments;

    sync_employment_info_size =
        community__sync_employment_info__get_packed_size( &sync_employment_info );

    char *sync_employment_info_buff = xmalloc( sync_employment_info_size );
    size_t rc_size =
        community__sync_employment_info__pack( &sync_employment_info, sync_employment_info_buff );
    assert( sync_employment_info_size == rc_size );

    char *server_url = "http://127.0.0.1:8080/syncemploymentinfo";
    http_post_pb(sync_employment_info_buff, rc_size, server_url);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

