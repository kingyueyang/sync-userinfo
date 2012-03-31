/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  utils for test
 *
 *        Version:  1.0
 *        Created:  03/31/2012 10:26:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <curl/curl.h>

void die(const char *format, ...);
void *xmalloc(size_t size);
void *xrealloc(void *a, size_t size);
void xfree( void *p);
char *xstrdup(const char *str);
int http_post_pb(char *buf, size_t size, char *post_url); 

