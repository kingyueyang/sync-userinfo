/*
 * =====================================================================================
 *
 *       Filename:  queue.h
 *
 *    Description:  receiver msg queue
 *
 *        Version:  1.0
 *        Created:  04/05/2012 10:08:11 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "utils.h"

typedef struct _queue_item
{
	char *action;
	void *data;
	size_t sz;
	
	struct _queue_item *next;
} QUEUE_ITEM;

typedef struct _queue
{
	size_t numitems;
	QUEUE_ITEM *items;
	
	pthread_mutex_t modify_mutex;
	pthread_mutex_t read_mutex;
} QUEUE;

QUEUE *initialize_queue(void);
void add_queue_item(QUEUE *, char *, void *, size_t);
QUEUE_ITEM *get_queue_item(QUEUE *);
void free_queue_item(QUEUE_ITEM *);

#endif

