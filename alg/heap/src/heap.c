#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"

//////////////////////////////////////////
// Useful macros for pointer arithmetic //
//////////////////////////////////////////

// Returns the address of the element with a certain index in the heap
#define index(H,X) ((H)->root + (X) * (H)->size)

// Returns the address of the left child of a certain index in the heap
#define lch(H,X) (index((H), 2 * (X)))

// Returns the address of the right child of a certain index in the heap
#define rch(H,X) (index((H), 2 * (X) + 1))

// Returns the address of the parent of a certain index in the heap
#define parent(H,X) (index((H), (X) / 2))

/**
 * The heap is a very useful data structure for situations in which you fre-
 * quently need the smallest (or largest) element. Here, it is defined generi-
 * cally, which creates the need for a field to store the size of each element
 * as well as a function pointer to store a comparison function. This function
 * is responsible for sorting the heap, similarly to the way stdlib's qsort
 * works.
 * 
typedef struct {
    unsigned int size;
    int (*compar)(const void *a, const void *b);
    unsigned long long len;
    unsigned long long num;
    void *root;
} Heap;
 */

/**
 * Initializes a new empty heap with a fixed size.
 * @param  size   Size, in bytes, of each element.
 * @param  len    Length of the new heap (maximum number of elements)
 * @param  compar Comparison function which should be used to sort the elements
 * @return        Address of the new heap.
 *
 */
Heap *hInit(unsigned int size, unsigned long long len,
            int (*compar)(const void *a, const void *b)){
    Heap *new = malloc(sizeof(Heap));
    new->size = size;
    new->len = len;
    new->num = 0;
    new->compar = compar;
    new->root = malloc(size * len) - size; // 1-based indexing
    new->swapbuffer = malloc(size);
    return new;
}

/**
 * Inserts an element into the heap.
 * @param h    Heap in which the element shall be inserted.
 * @param data Address in which the data to be inserted is located.
 */
void hInsert(Heap *h, void *data){
    if (h->num >= h->len){
        fprintf(stderr, "ERROR: Attempt to insert element into full heap (h = %p)\n", h);
        return;
    }
    h->num += 1;
    int cur = h->num;
    // Adds the new element after the last element in the heap.
    memcpy(index(h, h->num), data, h->size);
    // Swaps the new node up until it gets to the root or becomes
    // larger than its parent (in case of a min-heap).
    while (cur > 1 && h->compar(index(h, cur), parent(h, cur)) < 0){
        memcpy(h->swapbuffer, index(h, cur), h->size);
        memcpy(index(h, cur), parent(h, cur), h->size);
        memcpy(parent(h, cur), h->swapbuffer, h->size);
        cur /= 2;
    }
}

/**
 * Removes an element from the heap and copies its data to a given address.
 * @param h   Heap from which the element shall be removed.
 * @param out Address to which the data shall be written.
 */
void hPop(Heap *h, void *out){
    if (h->num <= 0){
        fprintf(stderr, "ERROR: Attempt to remove element from empty heap (h = %p)\n", h);
        return;
    }
    int cur = 1;
    // Copies the root element to the output address
    memcpy(out, index(h, 1), h->size);
    // Moves the last element in the heap to the root
    if (h->num == 1){
        h->num = 0;
        return;
    }
    memcpy(index(h, 1), index(h, h->num), h->size);
    h->num -= 1;
    while ((cur * 2) <= h->num){
        int smallest_child;
        if (h->num > cur * 2 && h->compar(lch(h, cur), rch(h, cur)) > 0)
            smallest_child = cur * 2 + 1;
        else
            smallest_child = cur * 2;
        // Swaps the new root with its smallest child in case it's larger than
        // the child node until it becomes a leaf node or the condition is no
        // longer true (once again considering the case of a min-heap)
        if (h->compar(index(h, cur), index(h, smallest_child)) > 0){
            memcpy(h->swapbuffer, index(h, cur), h->size);
            memcpy(index(h, cur), index(h, smallest_child), h->size);
            memcpy(index(h, smallest_child), h->swapbuffer, h->size);
            cur = smallest_child;
        }
        else break;
    }  
}

/**
 * Empties the heap.
 * @param  h Heap to be emptied.
 */
void hEmpty(Heap *h){
    h->num = 0;
}

/**
 * Returns the number of elements in the heap.
 * @param  h Heap to be analyzed.
 * @return   Number of elements in the heap.
 */
int hNum(Heap *h){
    return h->num;
}

/**
 * Frees the memory allocated for the heap and deletes it.
 * @param h Heap to be deleted.
 */
void hDelete(Heap *h){
    free(h->root + h->size);
    free(h->swapbuffer);
    free(h);
}