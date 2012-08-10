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

    char *update_proto;
    char *delete_proto;
    char *insert_proto;

    int mysql_query_rc;
    int flag;

    void *pop_string;
    char *raw_string;
    char *tmp;
    unsigned long long affect;
    int raw_len;

    MYSQL mysql;

    my_bool reconnect = 1;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);

    /* Connect to mysql server */
    if(!mysql_real_connect(
                &mysql, server.mysqlIP, server.mysqlUser,
                server.mysqlPasswd, server.db, MYSQL_PORT, NULL, 0)) {
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

    if(!mysql_ping(&mysql)) {
        mysql_query(&mysql, "SET NAMES UTF8");
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_TRACE,
                "MySQL_conn: Mysql Server set utf-8");
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_INFO,
                "MySQL_conn: %s", "SET NAMES UTF8");
    } else {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "MySQL_conn: lost connect to Mysql Server");
        return ((void *)0);
    }

    int malloc_size;

    while(1) {
        update_proto = NULL;
        delete_proto = NULL;
        insert_proto = NULL;
        if (apr_queue_pop(queue, &pop_string) != APR_SUCCESS)
            continue;

        raw_string = pop_string;
        if(NULL == raw_string) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_NOTICE,
                    "MySQL_conn: POP from queue is NULL");
            continue;
        }

        raw_len = strlen(raw_string);
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_TRACE,
                "MySQL_conn: POP from queue: %s", raw_string);

        /* Get flag */
        tmp = strsep(&raw_string, ":");
        flag= atoi(tmp);

        /* Keep elegant */
        /* If Basic Info */
        if(1 == flag) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_basic: post basic");
            /* Split raw string */
            uid = strsep(&raw_string, ",");
/*TODO: chaned: YYYY-MM-DD*/
            birth_year = strsep(&raw_string, ",");
            birth_month = strsep(&raw_string, ",");
            birth_day = strsep(&raw_string, ",");

            constellation = strsep(&raw_string, ",");

            blood_types = strsep(&raw_string, ",");
            sex = strsep(&raw_string, ",");

            home_nation = strsep(&raw_string, ",");
            if(strcmp("国家/地区", home_nation) == 0) {
		    home_nation = "";
	    }

            home_pro = strsep(&raw_string, ",");
            if(strcmp("省份", home_pro) == 0){
		    home_pro= "";
	    }

            home_city = strsep(&raw_string, ",");
            if(strcmp("城市", home_city) == 0){
		    home_city= "";
	    }

            now_nation = strsep(&raw_string, ",");
            if(strcmp("国家/地区", now_nation) == 0){
		    now_nation = "";
	    }

            now_pro = strsep(&raw_string, ",");
            if(strcmp("省份", now_pro) == 0){
		    now_pro= "";
	    }

            now_city = strsep(&raw_string, ",");
            if(strcmp("城市", now_city) == 0){
		    now_city= "";
	    }

            /* Magic number 350 is SQL proto length */
            malloc_size = raw_len + 512;
            update_proto = xmalloc(malloc_size);
            if(NULL == update_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_basic: update alloc memory error");
                xfree(update_proto);
                continue;
            }
/*TODO: chaned: YYYY-MM-DD*/
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
            /* query mysql */
            if(!mysql_ping(&mysql)) {
                mysql_query(&mysql, "SET NAMES UTF8");
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: Mysql Server update return: %d",
                        mysql_query_rc);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_INFO,
                        "MySQL_conn_basic: %s", update_proto);
            } else {
                /* Dump to file */
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
                        "MySQL_conn_basic: update affect:%d", affect);
            }
            /* new user */
            if(0 == affect) {
                malloc_size = raw_len + 512;
                insert_proto = xmalloc(malloc_size);
                if(NULL == insert_proto) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_WARN,
                            "MySQL_conn_basic: insert alloc memory error");
                    xfree(insert_proto);
                    xfree(update_proto);
                    continue;
                }
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
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_basic: Mysql Server insert return: %d",
                            mysql_query_rc);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_INFO,
                            "MySQL_conn_basic: %s", insert_proto);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_basic: lost connect to Mysql Server");
                    /* Dump to file */
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_basic: insert affect:%d", affect);
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
            malloc_size = raw_len + 128;
            update_proto = xmalloc(malloc_size);
            if(NULL == update_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_header: update alloc memory error");
                xfree(update_proto);
                continue;
            }
            snprintf(update_proto, (malloc_size),
                    "update base_user_info set header=%s where uid=%s",
                    header, uid);
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_header: updata proto: %s", update_proto);
            if(!mysql_ping(&mysql)) {
                mysql_query(&mysql, "SET NAMES UTF8");
                mysql_query_rc = mysql_query(&mysql, update_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: Mysql Server update return: %d",
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
                        "MySQL_conn_header: update affect:%d", affect);
            }
            /* new user */
            if(0 == affect) {
                malloc_size = raw_len + 128;
                insert_proto = xmalloc(malloc_size);
                if(NULL == insert_proto) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_WARN,
                            "MySQL_conn_header: insert alloc memory error");
                    xfree(insert_proto);
                    xfree(update_proto);
                    continue;
                }
                snprintf(insert_proto, malloc_size,
                        "insert into base_user_info set\
                        header=%s, uid=%s", 
                        header, uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: insert proto: %s", insert_proto);

                if(!mysql_ping(&mysql)) {
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_header: Mysql Server insert return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_header: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_header: insert affect:%d", affect);
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
            malloc_size = 128;
            delete_proto = xmalloc(malloc_size);
            if(NULL == delete_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_edu: delete alloc memory error");
                xfree(delete_proto);
                continue;
            }

            snprintf(delete_proto, (malloc_size),
                    "delete from base_user_education where uid=%s", uid );
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_edu: delete proto: %s", delete_proto);
            if(!mysql_ping(&mysql)) {
                mysql_query(&mysql, "SET NAMES UTF8");
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: Mysql Server delete return: %d",
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
                        "MySQL_conn_edu: delete affect:%d", affect);
            }

            /* Magic number is SQL proto length plus uid length*/
            malloc_size = raw_len + 512;
            insert_proto = xmalloc(malloc_size);
            if(NULL == insert_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_edu: insert alloc memory error");
                xfree(delete_proto);
                xfree(insert_proto);
                continue;
            }

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
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_edu: Mysql Server insert return: %d",
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
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_edu: AFFECT:%d", affect);
                malloc_size = raw_len + 128;
                insert_proto = xmalloc(malloc_size);
                if(NULL == insert_proto) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_WARN,
                            "MySQL_conn_edu: insert alloc memory error");
                    xfree(delete_proto);
                    xfree(insert_proto);
                    continue;
                }
                snprintf(insert_proto, malloc_size,
                        "insert into base_user_info set\
                        uid=%s", uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_edu: insert proto: %s", insert_proto);

                if(!mysql_ping(&mysql)) {
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_edu: Mysql Server insert return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_edu: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
                //notify_rt("S001", uid);
                //notify_rt("S002", uid);
                //notify_rt("S003", uid);
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

            malloc_size = 128;
            /* Magic number is SQL proto length plus uid length*/
            delete_proto = xmalloc(malloc_size);
            if(NULL == delete_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_emp: delete alloc memory error");
                xfree(delete_proto);
                continue;
            }

            snprintf(delete_proto, (malloc_size),
                    "delete from base_user_employment where uid=%s", uid );
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "MySQL_conn_emp: delete proto: %s", delete_proto);
            if(!mysql_ping(&mysql)) {
                mysql_query(&mysql, "SET NAMES UTF8");
                mysql_query_rc = mysql_query(&mysql, delete_proto);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: Mysql Server delete return: %d",
                        mysql_query_rc);
            } else {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_emp: lost connect to Mysql Server");
                fprintf(server.dump_file_handler, "%s\n", delete_proto);
                fflush(server.dump_file_handler);
            }

            if(mysql_field_count(&mysql) == 0) {
                affect = (unsigned long long )mysql_affected_rows(&mysql);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: delete affect:%d", affect);
            }

            /* Magic number is SQL proto length plus uid length*/
            malloc_size = raw_len + 512;
            insert_proto = xmalloc(malloc_size);
            if(NULL == insert_proto) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_WARN,
                        "MySQL_conn_emp: insert alloc memory error");
                xfree(delete_proto);
                xfree(insert_proto);
                continue;
            }

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
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_emp: Mysql Server insert return: %d",
                            mysql_query_rc);
                } else {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_ERROR,
                            "MySQL_conn_emp: lost connect to Mysql Server");
                    fprintf(server.dump_file_handler, "%s\n", insert_proto);
                    fflush(server.dump_file_handler);
                }
            }

            if(affect == 0) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "MySQL_conn_emp: AFFECT:%d", affect);
                malloc_size = raw_len + 128;
                insert_proto = xmalloc(malloc_size);
                if(NULL == insert_proto) {
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_WARN,
                            "MySQL_conn_emp: insert alloc memory error");
                    xfree(delete_proto);
                    xfree(insert_proto);
                    continue;
                }
                snprintf(insert_proto, malloc_size,
                        "insert into base_user_info set\
                        uid=%s", uid);
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_TRACE,
                        "MySQL_conn_emp: insert proto: %s", insert_proto);

                if(!mysql_ping(&mysql)) {
                    mysql_query(&mysql, "SET NAMES UTF8");
                    mysql_query_rc = mysql_query(&mysql, insert_proto);
                    log4c_category_log(
                            log_handler, LOG4C_PRIORITY_TRACE,
                            "MySQL_conn_emp: Mysql Server insert return: %d",
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

