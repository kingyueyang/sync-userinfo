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
    int flag = 0;
    /* TODO: pop itme from queue */
    /*char pop_string[] = "1:407902377,1987,12,21,0,2,2,china,hansu,lanzhou,china,beijing,朝阳";*/
    /*char pop_string[] = "2:407902377,1";*/
    /*char pop_string[] = "3:407902377;2,000000716,0000007160005,0,2008;4,100011178,100011178,0,2003";*/
    char pop_string[] = "4:407902377;2000,2,2009,4,FCT,100107;1994,2,199,4,TTT,100106";

    char *raw_string = pop_string;

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

    /* Try Pop: Nonblock */
    flag= atoi(strsep(&raw_string, ":"));

    /* If Basic Info */
    if(1 == flag) {
        printf ( "flag:%d\n", flag );
        /* Split raw string */
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

        /* FIXME:strlen(raw) + 333 */
        update_proto = malloc(10240);
        snprintf(update_proto, 10240, "update base_user_info set \
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
        if(!mysql_ping(&mysql)){
            printf("%s\n", update_proto);
            mysql_query_rc = mysql_query(&mysql, update_proto);
            printf ( "%d\n", mysql_query_rc );
        } else {
            /* Dump to file */
            /* fprintf(dump_file, "%s\n", update_proto); */
            /* fflush(dump_file); */
        }
        free(update_proto);
        update_proto = NULL;
    }

    /* If Header Info */
    if(2 == flag) {
        printf ( "flag:%d\n", flag );
        uid = strsep(&raw_string, ",");
        header = strsep(&raw_string, ",");

        /* FIXME:strlen(raw) + 66 */
        update_proto = malloc(512);
        snprintf(update_proto, 512, "update base_user_info set header=%s where uid=%s", header, uid);
        if(!mysql_ping(&mysql)) {
            printf("%s\n", update_proto);
            mysql_query_rc = mysql_query(&mysql, update_proto);
            printf ( "%d\n", mysql_query_rc );
        } else {
            /* Dump to file */
            /* fprintf(dump_file, "%s\n", update_proto); */
            /* fflush(dump_file); */
        }

        free(update_proto);
        update_proto = NULL;
    }

    /* If Education Info */
    if(3 == flag) {
        /*printf ( "%s\n", raw_string );*/
        printf ( "flag:%d\n", flag );
        uid = strsep(&raw_string, ";");

        delete_proto = malloc(512);
        insert_proto = malloc(512);
        snprintf(delete_proto, 512, "delete from base_user_education where uid=%s", uid );
        if(!mysql_ping(&mysql)) {
            printf("%s\n", delete_proto);
            mysql_query_rc = mysql_query(&mysql, delete_proto);
            printf ( "%d\n", mysql_query_rc );
        } else {
            /* Dump to file */
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

            snprintf(insert_proto, 512,
                    "insert into base_user_education set uid=%s, edu=%s,\
                    school='%s', department='%s', classes=%s, year=%s",
                    uid, edu, school, department, class_, year);
            if(!mysql_ping(&mysql)) {
                printf ( "%s\n", insert_proto );
                mysql_query_rc = mysql_query(&mysql, insert_proto);
                printf ( "%d\n", mysql_query_rc );
            } else {
                /* Dump to file */
            }
        }

        if(affect == 0) {
            /* Send notify */
        }

        free(delete_proto);
        free(insert_proto);
    }

    /* If employment Info */
    if(4 == flag) {
        /*printf ( "%s\n", raw_string );*/
        printf ( "flag:%d\n", flag );
        uid = strsep(&raw_string, ";");

        delete_proto = malloc(512);
        insert_proto = malloc(512);
        snprintf(delete_proto, 512, "delete from base_user_employment where uid=%s", uid );
        if(!mysql_ping(&mysql)) {
            printf("%s\n", delete_proto);
            mysql_query_rc = mysql_query(&mysql, delete_proto);
            printf ( "%d\n", mysql_query_rc );
        } else {
            /* Dump to file */
        }

        while( (neaf = strsep(&raw_string, ";")) != NULL ) {
            begin_year = strsep(&neaf, ",");
            begin_month = strsep(&neaf, ",");
            end_year = strsep(&neaf, ",");
            end_month = strsep(&neaf, ",");
            company = strsep(&neaf, ",");
            post = strsep(&neaf, ",");

            snprintf(insert_proto, 512,
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
                /* Dump to file */
            }
        }
        /* If not deltet, notiry real time */
        free(delete_proto);
        free(insert_proto);
    }

    if(flag > 4 || flag < 1) {
        printf ( "error\n" );
        /*continue;*/
    }

    return ((void *)0);
}

