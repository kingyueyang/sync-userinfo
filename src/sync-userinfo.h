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

#define	QUEUE_SIZE 10240000 /* Receiver Queue */

struct syncServer {
    char *receiverIP;
    char *dump_file;
    unsigned short receiverPort;
    unsigned int mysql_thread;
};

int initServerConfig(void);
int xdaemon(void);
int create_queue(void);
void create_thread();

extern struct syncServer server;
extern log4c_category_t* log_handler;

#endif

