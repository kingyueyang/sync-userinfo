/*
 * =====================================================================================
 *
 *       Filename:  inf_header_info.h
 *
 *    Description:  Sync user head portrait information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 06:34:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _INF_HEADER_INFO_H_
#define _INF_HEADER_INFO_H_

#include <stdio.h>
#include <stdlib.h>

#include "receiver.h"

/* 2 = comma and '\0' */
#define	HEADER_OFFSET 2			/* offset in output buff */

void post_SHI_cb(struct evhttp_request *req, void *arg);

#endif

