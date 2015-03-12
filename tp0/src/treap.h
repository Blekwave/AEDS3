/**
 * Treap - Abstract data type that combines features of a binary search tree
 * with those of a heap. Yields a sorted, unique structure with useful proper-
 * ties and efficient operations.
 */

#ifndef _TREAP_H_
#define _TREAP_H_

#include <stdbool.h>

/**
 * A treap node, which is equivalent to a size 1 treap. It is used to build
 * larger structures through pointers to child nodes.
 *
 * @param  key Key used for binary search tree sorting rules.
 * @param  pri Priority used for heap sorting rules. 
 * @param  left Pointer to child node to the left (smaller key and larger priori-
 *           ty than parent node)
 * @param  right Pointer to child node to the right (larger key and larger priori-
 *           ty than parent node)
 */
typedef struct struct_treap {
    int key;
    int pri;
    struct struct_treap *left;
    struct struct_treap *right;
} Treap;

/**
 * Initializes a Treap node with unique values for key and priority.
 * @param  k Key used for binary search tree sorting rules.
 * @param  p Priority used for heap sorting rules. 
 * @return   Address to the new Treap with NULL child pointers.
 */
Treap *Tr_Init(int key, int parent);

/**
 * Recursively deletes a Treap and all its elements.
 * @param treap Treap to be deleted.
 */
void Tr_Delete(Treap *treap);

/**
 * Merges two Treaps into a single Treap. The Treaps supplied for the operation
 * are not necessarily kept unchanged. The smallest element in one of the Treaps
 * must be larger than the largest element in the other Treap.
 * @param  a Treap to be merged.
 * @param  b Treap to be merged.
 * @return   Address of the topmost element of the merged treaps.
 */
Treap *Tr_Merge(Treap *a, Treap *b);

/**
 * Splits a Treap in two separate Treaps. The elements are split by their key
 * values, according to the split point supplied as a parameter.
 * @param source      Treap to be split.
 * @param smaller     Address to the new Treap, whose elements' keys are smaller
 *                    than or equal to the split point.
 * @param larger      Address to the new Treap, whose elements' keys are greater
 *                    than or equal to the split point. 
 * @param split_point Key value that may or may not correspond to an actual
 *                    element in the source Treap. Specifies the value used
 *                    to split the structure in two parts.
 */
void Tr_Split(Treap *source, Treap **smaller, Treap **larger, int split_point);

/**
 * Inserts a new node into a Treap. Can be used to create a new Treap by speci-
 * fying the first argument as NULL.
 * @param  treap Address of the Treap in which the new element shall be inserted.
 * @param  key   Key of the new node.
 * @param  pri   Priority of the new node.
 * @return       Address of the root of the Treap after insertion.
 */
Treap *Tr_Insert(Treap *treap, int key, int pri);

/**
 * Removes a node with a certain key from a Treap.
 * @param  treap Address of the Treap from which the element shall be removed.
 * @param  key   Key of the element to be removed.
 * @return       Address of the root of the Treap after removal.
 */
Treap *Tr_Remove(Treap *treap, int key);

/**
 * Prints directions to a node with a certain key in a Treap to stdout.
 * Directions are given starting from the root of the Treap. L means the child
 * node to the left of the current node, whereas R means the child node to the
 * right. -1 is returned in case no such node is found.
 * @param  treap Treap in which the search shall take place.
 * @param  key   Key of the element being searched for.
 * @return       true if the element exists, false if it doesn't. (Used for re-
 *               cursive implementation of the procedure, not relevant to the
 *               end user.)
 */
bool Tr_Locate(Treap *treap, int key, char *directions, int position,
    int *maxdepth);

#endif
