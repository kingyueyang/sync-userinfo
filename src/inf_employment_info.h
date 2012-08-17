/*
 * =====================================================================================
 *
 *       Filename:  inf_employment_info.h
 *
 *    Description:  Sync user employment information call back
 *
 *        Version:  1.0
 *        Created:  03/31/2012 09:49:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _INF_EMPLOYMENT_INFO_H_
#define _INF_EMPLOYMENT_INFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <apr_queue.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include "../proto/community.pb-c.h"
#include "logging.h"
#include "utils.h"

#include "gen_constellation.h"

void post_SMI_cb(struct evhttp_request *req, void *arg);
char *get_mobile(long unsigned int uid);

extern apr_pool_t *pool;
extern apr_queue_t *queue;

#endif

