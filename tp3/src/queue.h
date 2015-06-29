#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>
#define QUEUE_IS_EMPTY 1

typedef struct {
    struct queue_node *first;
    struct queue_node *last;
    size_t data_size;
} Queue;

Queue *qInit(size_t data_size);

void qEnqueue(Queue *q, void *data);

int qDequeue(Queue *q, void *output);

void qDelete(Queue *q);

#endif