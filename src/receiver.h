/*
 * =====================================================================================
 *
 *       Filename:  receiver.h
 *
 *    Description:  receiver sync information
 *
 *        Version:  1.0
 *        Created:  03/30/2012 04:19:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "sync-userinfo.h"
#include "inf_basic_info.h"
#include "inf_header_info.h"
#include "inf_education_info.h"
#include "inf_employment_info.h"
#include "logging.h"

void *receiver( void *args);
static void other_cb( struct evhttp_request *req, void *arg );

extern struct syncServer server;
extern log4c_category_t* log_handler;

#endif

