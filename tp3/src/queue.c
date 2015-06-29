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
    new->first = new->last = qNodeInit(0, NULL);
    new->first->next = NULL;
    return new;
}

void qEnqueue(Queue *q, void *data){
    struct queue_node *new = qNodeInit(q->data_size, data);
    new->next = NULL;
    q->last->next = new;
    q->last = new;
}

int qDequeue(Queue *q, void *output){
    if (q->first != q->last){
        struct queue_node *out = q->first->next;
        memcpy(output, out->data, q->data_size);
        if (q->last == out)
            q->last = q->first;
        q->first->next = out->next;
        qNodeDelete(out);
        return 0;
    }
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