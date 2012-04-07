/*
 * =====================================================================================
 *
 *       Filename:  msg_queue.h
 *
 *    Description:  message queue
 *
 *        Version:  1.0
 *        Created:  04/05/2012 04:05:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "receiver.h"

int msg_queue_server(void);

#endif
