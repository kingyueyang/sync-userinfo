/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/23/2011 03:18:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include	<stdio.h>
#include	<stdlib.h>
#include	<libconfig.h>

#include "logging.h"

int get_conf(const char *conf_file);

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
struct syncServer server_;

#endif
