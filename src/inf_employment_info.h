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

#include "../proto/community.pb-c.h"
#include "receiver.h"
#include "utils.h"

void post_SMI_cb(struct evhttp_request *req, void *arg);

#endif

