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

    FILE *dump_file_handle = NULL;

    MYSQL mysql;
    mysql_init(&mysql);

    /* Connect to mysql server */
    if(!mysql_real_connect(
                &mysql, "192.168.142.10", "ci_user",
                "y4hrthjUggg", "ci" , 3306, NULL, 0)) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_ERROR,
                "MySQL_conn: MySQL connecotr error");
        return ((void *)-1);
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_INFO,
            "MySQL_conn: MySQL connecotr start up");

    /* Turn on auto commint */
    mysql_autocommit(&mysql, 1);

    void *pop_string = NULL;
    char *raw_string = NULL;
    char *tmp = NULL;
    int raw_len = 1024;

    int malloc_size;

    /*dump_file_handle = fopen(server.dump_file, "a");*/
    /*if(NULL == dump_file_handle) {*/
    /*log4c_category_log(*/
            /*log_handler, LOG4C_PRIORITY_ERROR,*/
            /*"MySQL_conn: dump file open error");*/
        /*return ((void *)-2);*/
    /*}*/
    /*fprintf( "test\n");*/
    /*fflush(dump_file_handle);*/
    /*log4c_category_log(*/
            /*log_handler, LOG4C_PRIORITY_INFO,*/
            /*"MySQL_conn: dump file open ok");*/

    while(1) {
        apr_queue_pop(queue, &pop_string);

        raw_string = pop_string;
        raw_len = strlen(raw_string);

        if(NULL == raw_string) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_NOTICE,
                    "MySQL_conn: POP from queue is NULL");
            continue;
        }

        log4c_category_log(
                log_handler, LOG4C_PRIORITY_TRACE,
                "MySQL_conn: POP from queue: %s", raw_string);

        /* Try Pop: Nonblock */
        tmp = strsep(&raw_string, ":");
        flag= atoi(tmp);

        /* If Basic Info */
        if(1 == flag) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_basic: post basic");
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
            malloc_size = raw_len + 350;
            update_proto = xmalloc(malloc_size);
            snprintf(update_proto, (malloc_size), "update base_user_info set\
                    birth_year=%s, birth_month=%s, birth_day=%s,\
                    constellation=%s, blood_types=%s, sex=%s,\
                    home_nation='%s', home_pro='%s', home_city='%s',\
                    now_nation='%s', now_pro='%s', now_city='%s'\
                    where uid=%s", 
                    birth_year, birth_month, birth_day,
                    constellation, blood_types, sex,
                    home_nation, home_pro, home_city,
                    now_nation, now_pro, now_city,
                    uid);
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_basic: updata proto: %s", update_proto);
            if(!mysql_ping(&mysql)) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: connect Mysql ok");
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: Mysql Server return: %d", mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_basic: lost connect to Mysql Server");
                /* TODO: Dump to file */
            }

            xfree(update_proto);
        }

        /* If Header Info */
        if(2 == flag) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_header: post header");
            uid = strsep(&raw_string, ",");
            header = strsep(&raw_string, ",");

            /* Magic number 66 is SQL proto length */
            malloc_size = raw_len + 66;
            update_proto = xmalloc(malloc_size);
            snprintf(update_proto, (malloc_size),
                    "update base_user_info set header=%s where uid=%s",
                    header, uid);
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_header: updata proto: %s", update_proto);
            if(!mysql_ping(&mysql)) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: connect Mysql ok");
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: Mysql Server return: %d", mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_header: lost connect to Mysql Server");
                /* TODO: Dump to file */
            }

            xfree(update_proto);
        }

        /* If Education Info */
        if(3 == flag) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_edu: post education");
            uid = strsep(&raw_string, ";");

            /* Magic number is SQL proto length plus uid length*/
            malloc_size = 52;
            delete_proto = xmalloc(malloc_size);

            snprintf(delete_proto, (malloc_size),
                    "delete from base_user_education where uid=%s", uid );
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_edu: delete proto: %s", update_proto);
            if(!mysql_ping(&mysql)) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: connect Mysql ok");
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: Mysql Server return: %d", mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_edu: lost connect to Mysql Server");
                /* TODO: Dump to file */
            }

            unsigned long long affect;
            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
            }

            /* Magic number is SQL proto length plus uid length*/
            malloc_size = raw_len + 150;
            insert_proto = xmalloc(malloc_size);

            while( (neaf = strsep(&raw_string, ";")) != NULL ) {
                edu = strsep(&neaf, ",");
                school = strsep(&neaf, ",");
                department = strsep(&neaf, ",");
                class_ = strsep(&neaf, ",");
                year = strsep(&neaf, ",");

                snprintf(insert_proto, (malloc_size),
                        "insert into base_user_education set uid=%s, edu=%s,\
                        school='%s', department='%s', classes=%s, year=%s",
                        uid, edu, school, department, class_, year);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: insert proto: %s", insert_proto);
                if(!mysql_ping(&mysql)) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_edu: connect Mysql ok");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_edu: Mysql Server return: %d", mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_edu: lost connect to Mysql Server");
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
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_emp: post employment");
            uid = strsep(&raw_string, ";");

            malloc_size = 63;
            /* Magic number is SQL proto length plus uid length*/
            delete_proto = xmalloc(malloc_size);

            snprintf(delete_proto, (malloc_size), "delete from base_user_employment where uid=%s", uid );
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_emp: delete proto: %s", update_proto);
            if(!mysql_ping(&mysql)) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: connect Mysql ok");
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: Mysql Server return: %d", mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_emp: lost connect to Mysql Server");
                /* TODO: Dump to file */
            }

            /* Magic number is SQL proto length plus uid length*/
            malloc_size = raw_len + 202;
            insert_proto = xmalloc(malloc_size);

            while( (neaf = strsep(&raw_string, ";")) != NULL ) {
                begin_year = strsep(&neaf, ",");
                begin_month = strsep(&neaf, ",");
                end_year = strsep(&neaf, ",");
                end_month = strsep(&neaf, ",");
                company = strsep(&neaf, ",");
                post = strsep(&neaf, ",");

                snprintf(insert_proto, (raw_len + 202),
                        "insert into base_user_employment set company='%s',\
                        post='%s', begin_year=%s, begin_month=%s,\
                        end_year=%s, end_month=%s, uid= %s",
                        company, post, begin_year, begin_month,
                        end_year, end_month, uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: insert proto: %s", insert_proto);
                if(!mysql_ping(&mysql)) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_emp: connect Mysql ok");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_emp: Mysql Server return: %d", mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_emp: lost connect to Mysql Server");
                    /* TODO: Dump to file */
                }
            }
            /* If not deltet, notiry real time */
            xfree(delete_proto);
            xfree(insert_proto);
        }

        if(flag > 4 || flag < 1) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_NOTICE,
                    "MySQL_conn_other: flag error");
            xfree(pop_string);
            continue;
        }

        xfree(pop_string);
    }

    return ((void *)0);
}

