/*
 * =====================================================================================
 *
 *       Filename:  mysql_connector.c
 *
 *    Description:  connect to mysql and execute query
 *
 *        Version:  1.0
 *        Created:  04/11/2012 10:54:08 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "mysql_connector.h"

void *
mysql_connector(void *args) {
    char *neaf = NULL;

    char *uid = NULL;
    char *birth_year = NULL;
    char *birth_month = NULL;
    char *birth_day = NULL;
    char *constellation = NULL;
    char *blood_types = NULL;
    char *sex = NULL;
    char *home_nation = NULL;
    char *home_pro = NULL;
    char *home_city = NULL;
    char *now_nation = NULL;
    char *now_pro = NULL;
    char *now_city = NULL;

    char *header = NULL;

    char *edu = NULL;
    char *school = NULL;
    char *department = NULL;
    char *class_ = NULL;
    char *year = NULL;

    char *begin_year = NULL;
    char *begin_month = NULL;
    char *end_year = NULL;
    char *end_month = NULL;
    char *company = NULL;
    char *post = NULL;

    /* End char pointer define */

    char *update_proto = NULL;
    char *delete_proto = NULL;
    char *insert_proto = NULL;

    int mysql_query_rc;
    int flag;

    MYSQL mysql;
    mysql_init(&mysql);

    /* Connect to mysql server */
    if(!mysql_real_connect(
                &mysql, "192.168.142.10", "ci_user",
                "y4hrthjUggg", "ci" , 3306, NULL, 0)) {
        exit (-1);
    }

    /* Turn on auto commint */
    mysql_autocommit(&mysql, 1);

    void *pop_string = NULL;
    char *raw_string = NULL;
    char *tmp = NULL;
    int raw_len = 1024;
    while(1) {
        apr_queue_pop(queue, &pop_string);
        raw_string = pop_string;
        raw_len = strlen(raw_string);

        if(NULL == raw_string) {
            printf ( "continue\n" );
            continue;
        }

        /* Try Pop: Nonblock */
        tmp = strsep(&raw_string, ":");
        flag= atoi(tmp);

        /* If Basic Info */
        if(1 == flag) {
            /* Split raw string */
            /*FIXME: will add lock*/
            uid = strsep(&raw_string, ",");
            birth_year = strsep(&raw_string, ",");
            birth_month = strsep(&raw_string, ",");
            birth_day = strsep(&raw_string, ",");
            constellation = strsep(&raw_string, ",");

            blood_types = strsep(&raw_string, ",");
            sex = strsep(&raw_string, ",");

            home_nation = strsep(&raw_string, ",");
            home_pro = strsep(&raw_string, ",");
            home_city = strsep(&raw_string, ",");

            now_nation = strsep(&raw_string, ",");
            now_pro = strsep(&raw_string, ",");
            now_city = strsep(&raw_string, ",");

            /* Magic number 350 is SQL proto length */
            update_proto = xmalloc(raw_len + 350);
            snprintf(update_proto, (raw_len + 350), "update base_user_info set \
                    birth_year=%s, birth_month=%s, birth_day=%s, \
                    constellation=%s, blood_types=%s, sex=%s, \
                    home_nation='%s', home_pro='%s', home_city='%s', \
                    now_nation='%s', now_pro='%s', now_city='%s' \
                    where uid=%s", 
                    birth_year, birth_month, birth_day,
                    constellation, blood_types, sex,
                    home_nation, home_pro, home_city,
                    now_nation, now_pro, now_city,
                    uid);
            if(!mysql_ping(&mysql)) {
                printf("%s\n", update_proto);
                mysql_query_rc = mysql_query(&mysql, update_proto);
                printf ( "mysql_rc:%d\n", mysql_query_rc );
            } else {
                /* TODO: Dump to file */
            }

            xfree(update_proto);
        }

        /* If Header Info */
        if(2 == flag) {
            uid = strsep(&raw_string, ",");
            header = strsep(&raw_string, ",");

            /* Magic number 66 is SQL proto length */
            update_proto = xmalloc(raw_len + 66);
            snprintf(update_proto, (raw_len + 66), "update base_user_info set header=%s where uid=%s", header, uid);
            if(!mysql_ping(&mysql)) {
                printf("%s\n", update_proto);
                mysql_query_rc = mysql_query(&mysql, update_proto);
                printf ( "%d\n", mysql_query_rc );
            } else {
                /* TODO: Dump to file */
            }

            xfree(update_proto);
        }

        /* If Education Info */
        if(3 == flag) {
            uid = strsep(&raw_string, ";");

            /* Magic number is SQL proto length plus uid length*/
            delete_proto = xmalloc(42 + 10);
            /* Magic number is SQL proto length plus uid length*/
            insert_proto = xmalloc(raw_len + 150);
            snprintf(delete_proto, (42 + 10), "delete from base_user_education where uid=%s", uid );
            if(!mysql_ping(&mysql)) {
                printf("%s\n", delete_proto);
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                printf ( "%d\n", mysql_query_rc );
            } else {
                /* TODO: Dump to file */
            }

            unsigned long long affect;
            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
            }

            while( (neaf = strsep(&raw_string, ";")) != NULL ) {
                edu = strsep(&neaf, ",");
                school = strsep(&neaf, ",");
                department = strsep(&neaf, ",");
                class_ = strsep(&neaf, ",");
                year = strsep(&neaf, ",");

                snprintf(insert_proto, (raw_len + 150),
                        "insert into base_user_education set uid=%s, edu=%s,\
                        school='%s', department='%s', classes=%s, year=%s",
                        uid, edu, school, department, class_, year);
                if(!mysql_ping(&mysql)) {
                    printf ( "%s\n", insert_proto );
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    printf ( "%d\n", mysql_query_rc );
                } else {
                    /* TODO: Dump to file */
                }
            }

            if(affect == 0) {
                /*TODO: send notify*/
            }

            xfree(delete_proto);
            xfree(insert_proto);
        }

        /* If employment Info */
        if(4 == flag) {
            uid = strsep(&raw_string, ";");

            delete_proto = xmalloc(53 + 10);
            insert_proto = xmalloc(raw_len + 202);
            snprintf(delete_proto, (52 + 10), "delete from base_user_employment where uid=%s", uid );
            if(!mysql_ping(&mysql)) {
                printf("%s\n", delete_proto);
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                printf ( "%d\n", mysql_query_rc );
            } else {
                /* TODO: Dump to file */
            }

            while( (neaf = strsep(&raw_string, ";")) != NULL ) {
                begin_year = strsep(&neaf, ",");
                begin_month = strsep(&neaf, ",");
                end_year = strsep(&neaf, ",");
                end_month = strsep(&neaf, ",");
                company = strsep(&neaf, ",");
                post = strsep(&neaf, ",");

                snprintf(insert_proto, (raw_len + 202),
                        "insert into base_user_employment set company='%s', \
                        post='%s', begin_year=%s, begin_month=%s, \
                        end_year=%s, end_month=%s, uid= %s",
                        company, post, begin_year, begin_month,
                        end_year, end_month, uid);
                if(!mysql_ping(&mysql)) {
                    printf ( "%s\n", insert_proto );
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    printf ( "%d\n", mysql_query_rc );
                } else {
                    /* TODO: Dump to file */
                }
            }
            /* If not deltet, notiry real time */
            xfree(delete_proto);
            xfree(insert_proto);
        }

        if(flag > 4 || flag < 1) {
            xfree(pop_string);
            printf ( "flag error\n" );
            continue;
        }

        xfree(pop_string);
    }

    return ((void *)0);
}

