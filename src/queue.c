/*
 * =====================================================================================
 *
 *       Filename:  queue.c
 *
 *    Description:  receiver msg queue
 *
 *        Version:  1.0
 *        Created:  04/05/2012 10:07:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "queue.h"

QUEUE *
initialize_queue(void) {
	QUEUE *q;
	
	q = xmalloc(sizeof(QUEUE));
	if (!(q))
		return NULL;
	
	pthread_mutex_init(&(q->modify_mutex), NULL);
	pthread_mutex_init(&(q->read_mutex), NULL);
	pthread_mutex_lock(&(q->read_mutex));
	
	return q;
}

void 
add_queue_item(QUEUE *queue, char *action, void *data, size_t sz) {
	QUEUE_ITEM *qi;
	
	qi = xmalloc(sizeof(QUEUE_ITEM));
	if (!(qi))
		return;
	
	qi->data = data;
	qi->sz = sz;
	qi->action = xstrdup(action);
	
	pthread_mutex_lock(&(queue->modify_mutex));
	
	qi->next = queue->items;
	queue->items = qi;
	queue->numitems++;
	
	pthread_mutex_unlock(&(queue->modify_mutex));
	pthread_mutex_unlock(&(queue->read_mutex));
}

QUEUE_ITEM *
get_queue_item(QUEUE *queue) {
	QUEUE_ITEM *qi, *pqi;
	
	pthread_mutex_lock(&(queue->read_mutex));
	pthread_mutex_lock(&(queue->modify_mutex));
	
  if(!(qi = pqi = queue->items)) {
      return (QUEUE_ITEM *)NULL;
  }

  while ((qi->next))
  {
      pqi = qi;
      qi = qi->next;
  }

  pqi->next = NULL;
  if (queue->numitems == 1)
      queue->items = NULL;

  queue->numitems--;

  if (queue->numitems > 0)
      pthread_mutex_unlock(&(queue->read_mutex));

  pthread_mutex_unlock(&(queue->modify_mutex));

  return qi;
}

void 
free_queue_item(QUEUE_ITEM *queue_item) {
    free(queue_item->action);
    free(queue_item);
}

