/*
 * =====================================================================================
 *
 *       Filename:  inf_basic_info.h
 *
 *    Description:  Sync user basic information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 05:53:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _INF_BASIC_INFO_H_
#define _INF_BASIC_INFO_H_

#include <stdio.h>
#include <stdlib.h>

#include "../proto/community.pb-c.h"
#include "receiver.h"
#include "utils.h"

void post_SBI_cb(struct evhttp_request *req, void *arg);

extern apr_pool_t *pool;
extern apr_queue_t *queue;

#endif

