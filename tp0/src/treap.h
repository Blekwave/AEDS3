/**
 * Treap - Abstract data type that combines features of a binary search tree
 * with those of a heap. Yields a sorted, unique structure with useful proper-
 * ties and efficient operations.
 */

////////////////////////////////////////////////////////////////////////////////
// Note: up to date comments will not be kept in this file, but in the actual //
// source files. Comments tend to become outdated, and having to keep them in //
// two different places is clearly recipe for disaster.                       //
////////////////////////////////////////////////////////////////////////////////

#ifndef _TREAP_H_
#define _TREAP_H_

#include <stdbool.h>

typedef struct struct_treap {
    int key;
    int pri;
    struct struct_treap *left;
    struct struct_treap *right;
} Treap;

Treap *Tr_Init(int key, int parent);

void Tr_Delete(Treap *treap);

Treap *Tr_Merge(Treap *a, Treap *b);

void Tr_Split(Treap *source, Treap **smaller, Treap **larger, int split_point);

Treap *Tr_Insert(Treap *treap, int key, int pri);

Treap *Tr_Remove(Treap *treap, int key);

void Tr_Locate(Treap *treap, int key);

#endif
