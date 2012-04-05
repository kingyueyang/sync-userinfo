/*
 * =====================================================================================
 *
 *       Filename:  inf_educatoin_info.h
 *
 *    Description:  Sync user education information call back
 *
 *        Version:  1.0
 *        Created:  03/31/2012 07:57:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _INF_EDUCATION_INFO_H_
#define _INF_EDUCATION_INFO_H_

#include <stdio.h>
#include <stdlib.h>

#include "../proto/community.pb-c.h"
#include "receiver.h"
#include "utils.h"
#include "queue.h"

void post_SEI_cb(struct evhttp_request *req, void *arg);

extern QUEUE *queue;

#endif

