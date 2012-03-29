/*
 * =====================================================================================
 *
 *       Filename:  test_user_basic_info.h
 *
 *    Description:  send user basic info to sync user system
 *
 *        Version:  1.0
 *        Created:  03/29/2012 02:27:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _TEST_USER_BASIC_INFO_H_
#define _TEST_USER_BASIC_INFO_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include  <curl/curl.h>

#include "../../proto/community.pb-c.h"

static void die(const char *format, ...);
static void *xmalloc(size_t size);
static void *xrealloc(void *a, size_t size);
static char *xstrdup(const char *str);
int http_post_pb(char *buf, size_t size, char *post_url); 

#endif
