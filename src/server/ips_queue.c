#include "ips_queue.h"

queue *queue_init (void)
{
	queue *q;

	q = (queue *)malloc (sizeof (queue));
	if (q == NULL) return (NULL);

	q->empty = 1;
	q->full = 0;
	q->head = 0;
	q->tail = 0;
	q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
	pthread_mutex_init (q->mut, NULL);
	q->not_full = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
	pthread_cond_init (q->not_full, NULL);
	q->not_empty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
	pthread_cond_init (q->not_empty, NULL);
	
	return (q);
}

void queue_delete (queue *q)
{
	pthread_mutex_destroy (q->mut);
	free (q->mut);	
	pthread_cond_destroy (q->not_full);
	free (q->not_full);
	pthread_cond_destroy (q->not_empty);
	free (q->not_empty);
	free (q);
}

void queue_add (queue *q, int in)
{
	q->buf[q->tail] = in;
	q->tail++;
	if (q->tail == QUEUESIZE)
		q->tail = 0;
	if (q->tail == q->head)
		q->full = 1;
	q->empty = 0;

	return;
}

void queue_del (queue *q, int *out)
{
	*out = q->buf[q->head];

	q->head++;
	if (q->head == QUEUESIZE)
		q->head = 0;
	if (q->head == q->tail)
		q->empty = 1;
	q->full = 0;

	return;
}
