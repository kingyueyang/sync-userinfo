/*
 * =====================================================================================
 *
 *       Filename:  notify.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2012 03:30:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include <stdio.h>
#include <curl/curl.h>

#include "utils.h"
#include "receiver.h"

int notify_rt(char *type, char *uid);

#endif
