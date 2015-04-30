#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"

#define index(H,X) ((H)->root + (X) * (H)->size)

#define lch(H,X) (index((H), 2 * (X)))
#define rch(H,X) (index((H), 2 * (X) + 1))
#define parent(H,X) (index((H), (X) / 2))

Heap *hInit(unsigned int size, unsigned long long len,
            long long (*compar)(const void *a, const void *b)){
    Heap *new = malloc(sizeof(Heap));
    new->size = size;
    new->len = len;
    new->num = 0;
    new->compar = compar;
    new->root = malloc(size * len) - size; // 1-based indexing
    return new;
}

void hInsert(Heap *h, void *data){
    if (h->num >= h->len){
        fprintf(stderr, "ERROR: Attempt to insert element into full heap (h = %p)\n", h);
        return;
    }
    h->num += 1;
    int cur = h->num;
    memcpy(index(h, h->num), data, h->size);
    void *buffer = malloc(h->size);
    while (cur > 1 && h->compar(index(h, cur), parent(h, cur)) < 0){
        memcpy(buffer, index(h, cur), h->size);
        memcpy(index(h, cur), parent(h, cur), h->size);
        memcpy(parent(h, cur), buffer, h->size);
        cur /= 2;
    }
    free(buffer);
}

void hPop(Heap *h, void *out){
    if (h->num <= 0){
        fprintf(stderr, "ERROR: Attempt to remove element from empty heap (h = %p)\n", h);
        return;
    }
    int cur = 1;
    memcpy(out, index(h, 1), h->size);
    memcpy(index(h, 1), index(h, h->num), h->size);
    h->num -= 1;
    void *buffer = malloc(h->size);
    while ((cur * 2) <= h->num){
        int smallest_child;
        if (h->num > cur * 2 && h->compar(lch(h, cur), rch(h, cur)) > 0)
            smallest_child = cur * 2 + 1;
        else
            smallest_child = cur * 2;
        if (h->compar(index(h, cur), index(h, smallest_child)) > 0){
            memcpy(buffer, index(h, cur), h->size);
            memcpy(index(h, cur), index(h, smallest_child), h->size);
            memcpy(index(h, smallest_child), buffer, h->size);
            cur = smallest_child;
        }
        else break;
    }  
    free(buffer);
}

int hNum(Heap *h){
    return h->num;
}

void hDelete(Heap *h){
    free(h->root + h->size);
    free(h);
}