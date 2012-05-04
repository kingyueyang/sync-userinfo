/*
 * =====================================================================================
 *
 *       Filename:  sync-userinfo.h
 *
 *    Description:  sync user infornation
 *
 *        Version:  1.0
 *        Created:  03/30/2012 02:55:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _SYNC_USERINFO_H_
#define _SYNC_USERINFO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <apr-1/apr_queue.h>

#include "receiver.h"
#include "mysql_connector.h"
#include "logging.h"
#include "config.h"

//#define	QUEUE_SIZE 80960000 [> Receiver Queue <]
#define	QUEUE_SIZE 102400 /* Receiver Queue */

struct syncServer {
    const char *receiverIP;
    int receiverPort;
    const char *dump_file;

    const char *mysqlIP;
    const char *mysqlUser;
    const char *mysqlPasswd;
    const char *db;
    int mysqlPort;
    int mysqlThread;

    FILE *dump_file_handler;
};

int initServerConfig(void);
int xdaemon(void);
int create_queue(void);
void create_thread();

extern struct syncServer server;
extern log4c_category_t* log_handler;

struct mysql_item {
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
} mysql_itmes[20];

int create_dump(void);

#endif

