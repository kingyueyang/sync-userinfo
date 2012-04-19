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
    const char *receiver_ip;
    unsigned int receiver_port;
} receiver_cfg;

/* mysql configure */
typedef struct {
    const char *mysql_ip;
    const char *mysql_user;
    const char *mysql_pwd;
    const char *mysql_db;
    unsigned int mysql_port;
    unsigned int mysql_thread_count;
} mysql_cfg;

int get_conf(const char *conf_file);

