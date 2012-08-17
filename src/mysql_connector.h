/*
 * =====================================================================================
 *
 *       Filename:  mysql_connector.h
 *
 *    Description:  connect to mysql and execute query
 *
 *        Version:  1.0
 *        Created:  04/11/2012 10:56:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _MYSQL_CONNECTOR_H_
#define _MYSQL_CONNECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <apr_queue.h>

#include "utils.h"
#include "logging.h"
#include "sync-userinfo.h"

void *mysql_connector(void *args);

extern apr_pool_t *pool;
extern apr_queue_t *queue;
extern struct syncServer server;

#endif
