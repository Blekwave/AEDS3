#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct queue_node {
    struct queue_node *next;
    void *data;
};

static struct queue_node *qNodeInit(size_t data_size, void *data){
    struct queue_node *new = malloc(sizeof(struct queue_node));
    new->data = malloc(data_size);
    memcpy(new->data, data, data_size);
    return new;
};

static void qNodeDelete(struct queue_node *node){
    free(node->data);
    free(node);
}

Queue *qInit(size_t data_size){
    Queue *new = malloc(sizeof(Queue));
    new->data_size = data_size;
    pthread_mutex_init(&new->enqueue_mutex, NULL);
    pthread_mutex_init(&new->dequeue_mutex, NULL);
    new->first = new->last = qNodeInit(0, NULL);
    new->first->next = NULL;
    return new;
}

void qEnqueue(Queue *q, void *data){
    struct queue_node *new = qNodeInit(q->data_size, data);
    new->next = NULL;
    pthread_mutex_lock(&q->enqueue_mutex);
    q->last->next = new;
    q->last = new;
    pthread_mutex_unlock(&q->enqueue_mutex);
}

int qDequeue(Queue *q, void *output){
    struct queue_node *first, *next;
    pthread_mutex_lock(&q->dequeue_mutex);
    first = q->first;
    next = first->next;
    if (next != NULL){
        q->first = next;
        pthread_mutex_unlock(&q->dequeue_mutex);
        memcpy(output, next->data, q->data_size);
        qNodeDelete(first);
        return 0;
    }
    pthread_mutex_unlock(&q->dequeue_mutex);
    return QUEUE_IS_EMPTY;
}

void qDelete(Queue *q){
    struct queue_node *cur = q->first, *next;
    do {
        next = cur->next;
        qNodeDelete(cur);
        cur = next;
    } while (cur);
    free(q);
}