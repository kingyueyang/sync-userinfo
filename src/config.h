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

#include	<stdio.h>
#include	<stdlib.h>
#include	<libconfig.h>

/* receiver configure */
typedef struct {
} receiver_cfg;

/* mysql configure */
typedef struct {
} mysql_cfg;

conf get_conf(const char *conf_file);

