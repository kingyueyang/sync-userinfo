/*
 * =====================================================================================
 *
 *       Filename:  test_user_education_info.c
 *
 *    Description:  send user education info to sync user system
 *
 *        Version:  1.0
 *        Created:  03/31/2012 05:34:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "test_user_education_info.h"

int
main( int argc, char *argv[] ) {
    size_t sync_education_info_size;
    int64_t uid = 1057346345;
    int32_t edu[] = {2, 3};
    char school[][16] = {"junior school", "senior school"};
    char department[][16] = {"information", "math"};
    int32_t class[] = {1, 2};
    int32_t year[] = {2000, 2003};

    /* Education packages */
    Community__Education **educations;
    educations = xmalloc(sizeof(Community__Education) * 2);

    for(int i = 0; i < 2; i++) {
        educations[i] = xmalloc(sizeof(Community__Education));
        community__education__init(educations[i]);
        educations[i]->edu = edu[i];
        educations[i]->school = school[i];
        educations[i]->department = department[i];
        educations[i]->class_ = class[i];
        educations[i]->year = year[i];
    }

    Community__SyncEducationInfo sync_education_info =
        COMMUNITY__SYNC_EDUCATION_INFO__INIT;
    sync_education_info.uid = uid;
    sync_education_info.n_educations = 2;
    sync_education_info.educations = educations;

    sync_education_info_size =
        community__sync_education_info__get_packed_size( &sync_education_info );

    char *sync_education_info_buff = xmalloc( sync_education_info_size );
    size_t rc_size =
        community__sync_education_info__pack( &sync_education_info, sync_education_info_buff );
    assert( sync_education_info_size == rc_size );

    char *server_url = "http://127.0.0.1:8080/synceducationinfo";
    http_post_pb(sync_education_info_buff, rc_size, server_url);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

