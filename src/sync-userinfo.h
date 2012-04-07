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
#include <sys/types.h>

#include <apr-1/apr_queue.h>

#include "receiver.h"
#include "msg_queue.h"

struct syncServer {
    char *receiverIP;
    int receiverPort;
};

void initServerConfig( void );
void xdaemon( void );
int dofork( size_t fork_no, int flag );

extern struct syncServer server;

#endif
