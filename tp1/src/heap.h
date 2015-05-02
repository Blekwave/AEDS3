#ifndef _HEAP_H_
#define _HEAP_H_

#include <string.h>

typedef struct {
    unsigned int size;
    int (*compar)(const void *a, const void *b);
    unsigned long long len;
    unsigned long long num;
    void *root;
    void *swapbuffer;
} Heap;

Heap *hInit(unsigned int size, unsigned long long max,
            int (*compar)(const void *a, const void *b));

void hInsert(Heap *h, void *data);

void hPop(Heap *h, void *out);

int hNum(Heap *h);

void hDelete(Heap *h);

#endif