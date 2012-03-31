/*
 * =====================================================================================
 *
 *       Filename:  test_user_basic_info.c
 *
 *    Description:  send user basic info to sync user system
 *
 *        Version:  1.0
 *        Created:  03/29/2012 02:24:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "test_user_basic_info.h"

int
main( int argc, char *argv[] ) {
    int64_t uid = 1057346343;

    int32_t birth_year = 1987;
    int32_t birth_month = 2;
    int32_t birth_day = 24;
    int32_t constellation = 2;

    int32_t blood_types = 4;
    int32_t sex = 1;

    char home_nation[] = "China";
    char home_pro[] = "HeBei";
    char home_city[] = "ShiJiaZhuang";

    char now_nation[] = "China";
    char now_pro[] = "BeiJing";
    char now_city[] = "ChaoYang";

    Community__SyncBasicInfo sync_basic_info = COMMUNITY__SYNC_BASIC_INFO__INIT;

    sync_basic_info.uid = uid;

    sync_basic_info.birth_year = birth_year;
    sync_basic_info.birth_month = birth_month;
    sync_basic_info.birth_day = birth_day;
    sync_basic_info.constellation = constellation;

    sync_basic_info.blood_types = blood_types;
    sync_basic_info.sex = sex;

    sync_basic_info.home_nation = xstrdup(home_nation);
    sync_basic_info.home_pro = xstrdup(home_pro);
    sync_basic_info.home_city = xstrdup(home_city);

    sync_basic_info.now_nation = xstrdup(now_nation);
    sync_basic_info.now_pro = xstrdup(now_pro);
    sync_basic_info.now_city = xstrdup(now_city);

    size_t sync_basic_info_size =
        community__sync_basic_info__get_packed_size(&sync_basic_info);
    char *sync_basic_info_buff = xmalloc(sync_basic_info_size);

    size_t rc_size = 
        community__sync_basic_info__pack(&sync_basic_info, sync_basic_info_buff);
    assert(sync_basic_info_size == rc_size);
#if 0
    Community__SyncBasicInfo *test;
    test = community__sync_basic_info__unpack(NULL, rc_size, sync_basic_info_buff);
    printf ( "%d\n", test->uid);
    printf ( "%d\n", test->birth_year);
#endif

    char *server_url = "http://127.0.0.1:8080/syncbasicinfo";
    http_post_pb(sync_basic_info_buff, rc_size, server_url);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

