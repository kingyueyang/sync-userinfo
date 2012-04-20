/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/23/2011 03:18:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include	<stdio.h>
#include	<stdlib.h>
#include	<libconfig.h>

#include "sync-userinfo.h"

int get_conf(const char *conf_file);

extern struct syncServer server;

#endif
