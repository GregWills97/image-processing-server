#ifndef __IPS_QUEUE_H__
#define __IPS_QUEUE_H__

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define QUEUESIZE 6

typedef struct {
    int buf[QUEUESIZE];
    long head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *not_full, *not_empty;
} queue;

queue *queue_init (void);
void queue_delete (queue *q);
void queue_add (queue *q, int in);
void queue_del (queue *q, int *out);

#endif /*__IPS_QUEUE_H__*/
