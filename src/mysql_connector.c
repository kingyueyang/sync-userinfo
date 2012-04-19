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
    char *neaf;

    char *uid;
    char *birth_year;
    char *birth_month;
    char *birth_day;
    char *constellation;
    char *blood_types;
    char *sex;
    char *home_nation;
    char *home_pro;
    char *home_city;
    char *now_nation;
    char *now_pro;
    char *now_city;

    char *header;

    char *edu;
    char *school;
    char *department;
    char *class_;
    char *year;

    char *begin_year;
    char *begin_month;
    char *end_year;
    char *end_month;
    char *company;
    char *post;

    /* End char pointer define */

    char *update_proto;
    char *delete_proto;
    char *insert_proto;

    int mysql_query_rc;
    int flag;

    void *pop_string;
    char *raw_string;
    char *tmp;
    unsigned long long affect;
    int raw_len = 1024;

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

    int malloc_size;

    while(1) {
        update_proto = NULL;
        delete_proto = NULL;
        insert_proto = NULL;
        apr_queue_pop(queue, &pop_string);
/*log4c_category_log(*/
        /*log_handler, LOG4C_PRIORITY_DEBUG,*/
        /*"SQL: %s", pop_string);*/
/*continue;*/

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
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: Mysql Server return: %d",
                        mysql_query_rc);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_INFO,
                        "MySQL_conn_basic: %s", update_proto);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_basic: lost connect to Mysql Server");
                fprintf(server.dump_file_handler, "%s\n", update_proto);
                fflush(server.dump_file_handler);
            }

            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: is new user");
            }
            /* new user */
            if(0 == affect) {
                malloc_size = raw_len + 358;
                insert_proto = xmalloc(malloc_size);
                snprintf(insert_proto, malloc_size,
                        "insert into base_user_info set\
                        birth_year=%s, birth_month=%s, birth_day=%s,\
                        constellation=%s, blood_types=%s, sex=%s,\
                        home_nation='%s', home_pro='%s', home_city='%s',\
                        now_nation='%s', now_pro='%s', now_city='%s',\
                        uid=%s", 
                        birth_year, birth_month, birth_day,
                        constellation, blood_types, sex,
                        home_nation, home_pro, home_city,
                        now_nation, now_pro, now_city,
                        uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: insert proto: %s", insert_proto);
                if(!mysql_ping(&mysql)) {
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_basic: Mysql Server return: %d",
                            mysql_query_rc);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_INFO,
                            "MySQL_conn_basic: %s", insert_proto);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_basic: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
            }

            xfree(insert_proto);
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
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: Mysql Server return: %d",
                        mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_header: lost connect to Mysql Server");
                fprintf(server.dump_file_handler, "%s\n", update_proto);
                fflush(server.dump_file_handler);
            }

            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: is new user");
            }
            /* new user */
            if(0 == affect) {
                malloc_size = raw_len + 75;
                insert_proto = xmalloc(malloc_size);
                snprintf(insert_proto, malloc_size,
                        "insert into base_user_info set\
                        header=%s, uid=%s", 
                        header, uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: insert proto: %s", insert_proto);

                if(!mysql_ping(&mysql)) {
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_basic: Mysql Server return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_basic: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
            }

            xfree(insert_proto);
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
                    "MySQL_conn_edu: delete proto: %s", delete_proto);
            if(!mysql_ping(&mysql)) {
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: Mysql Server return: %d",
                        mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_edu: lost connect to Mysql Server");
                fprintf(server.dump_file_handler, "%s\n", delete_proto);
                fflush(server.dump_file_handler);
            }

            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_education: is new user");
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
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_edu: Mysql Server return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_edu: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
            }

            if(affect == 0) {
                if(0 == affect) {
                    malloc_size = raw_len + 75;
                    insert_proto = xmalloc(malloc_size);
                    snprintf(insert_proto, malloc_size,
                            "insert into base_user_info set\
                            header=%s, uid=%s", 
                            header, uid);
                }
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: insert proto: %s", insert_proto);

                if(!mysql_ping(&mysql)) {
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_basic: Mysql Server return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_basic: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
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

            snprintf(delete_proto, (malloc_size),
                    "delete from base_user_employment where uid=%s", uid );
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_emp: delete proto: %s", delete_proto);
            if(!mysql_ping(&mysql)) {
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: Mysql Server return: %d",
                        mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_emp: lost connect to Mysql Server");
                fprintf(server.dump_file_handler, "%s\n", delete_proto);
                fflush(server.dump_file_handler);
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
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_emp: Mysql Server return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_emp: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
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

