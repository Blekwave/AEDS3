#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>
#include <pthread.h>
#define QUEUE_IS_EMPTY 1

typedef struct {
    struct queue_node *first;
    struct queue_node *last;
    pthread_mutex_t enqueue_mutex;
    pthread_mutex_t dequeue_mutex;
    size_t data_size;
} Queue;

Queue *qInit(size_t data_size);

void qEnqueue(Queue *q, void *data);

int qDequeue(Queue *q, void *output);

void qDelete(Queue *q);

#endif