#ifndef _HEAP_H_
#define _HEAP_H_

#include <string.h>

typedef struct {
    size_t size;
    long long (*compar)(const void *a, const void *b);
    int len;
    int num;
    void *root;
} Heap;

Heap *hInit(size_t size, int max,
            long long (*compar)(const void *a, const void *b));

void hInsert(Heap *h, void *data);

void hPop(Heap *h, void *out);

int hNum(Heap *h);

void hDelete(Heap *h);

#endif