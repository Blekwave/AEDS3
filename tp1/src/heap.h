#ifndef _HEAP_H_
#define _HEAP_H_

#include <string.h>

typedef struct {
    unsigned int size;
    long long (*compar)(const void *a, const void *b);
    unsigned long long len;
    unsigned long long num;
    void *root;
} Heap;

Heap *hInit(unsigned int size, unsigned long long max,
            long long (*compar)(const void *a, const void *b));

void hInsert(Heap *h, void *data);

void hPop(Heap *h, void *out);

int hNum(Heap *h);

void hDelete(Heap *h);

#endif