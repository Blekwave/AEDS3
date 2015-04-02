/**
 * Treap - Abstract data type that combines features of a binary search tree
 * with those of a heap. Yields a sorted, unique structure with useful proper-
 * ties and efficient operations.
 */

#include "treap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
// typedef struct struct_treap {
//     int key;
//     int pri;
//     struct struct_treap *left;
//     struct struct_treap *right;
// } Treap;

/**
 * Initializes a Treap node with unique values for key and priority.
 * @param  k Key used for binary search tree sorting rules.
 * @param  p Priority used for heap sorting rules. 
 * @return   Address to the new Treap with NULL child pointers.
 */
Treap *Tr_Init(int key, int pri){
    Treap *new = malloc(sizeof(Treap));
    new->key = key;
    new->pri = pri;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void Tr_DeleteNode(Treap *treap){
    free(treap);
}

/**
 * Recursively deletes a Treap and all its elements.
 * @param treap Treap to be deleted.
 */
void Tr_Delete(Treap *treap){
    if (treap == NULL)
        return;
    Tr_Delete(treap->left);
    Tr_Delete(treap->right);
    Tr_DeleteNode(treap);
}

/**
 * Merges two Treaps, as long as one's node with the smallest key has a key
 * larger than the other Treap's node with the largest key.
 * @param  smaller Treap with the smaller nodes.
 * @param  larger  Treap with the larger nodes.
 * @return         Address of the root of the merged treaps.
 */
Treap *Tr_Merge(Treap *smaller, Treap *larger){
    Treap *root = NULL; // Of the merged tree;
    Treap **branch = &root;
    while (1){
        if (!(smaller && larger)){ // Finishes the procedure
            if (!smaller)
                *branch = larger;
            else
                *branch = smaller;
            break;
        } else if (larger->pri < smaller->pri){
            *branch = larger;
            branch = &larger->left;
            larger = larger->left;
        } else {
            *branch = smaller;
            branch = &smaller->right;
            smaller = smaller->right;
        }
    }
    return root;
}

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
void Tr_Split(Treap *source, Treap **smaller, Treap **larger, int split_point){
    Treap **s_branch = smaller, **l_branch = larger;
    *smaller = *larger = NULL;
    while (source && source->key != split_point){
        if (split_point > source->key){
            *s_branch = source;
            s_branch = &source->right;
            source = source->right;
        } else {
            *l_branch = source;
            l_branch = &source->left;
            source = source->left;
        }
    }
    *s_branch = source;
    if (source)
        *l_branch = source->right;
    else
        *l_branch = NULL;
}

/**
 * Inserts a new node into a Treap. Can be used to create a new Treap by speci-
 * fying the first argument as NULL.
 * @param  treap Address of the Treap in which the new element shall be inserted.
 * @param  key   Key of the new node.
 * @param  pri   Priority of the new node.
 * @return       Address of the root of the Treap after insertion.
 */
Treap *Tr_Insert(Treap *treap, int key, int pri){
    Treap *new = Tr_Init(key, pri), *smaller, *larger;
    Tr_Split(treap, &smaller, &larger, key);
    return Tr_Merge(Tr_Merge(smaller, new), larger);
}

/**
 * Removes a node with a certain key from a Treap.
 * @param  treap Address of the Treap from which the element shall be removed.
 * @param  key   Key of the element to be removed.
 * @return       Address of the root of the Treap after removal.
 */
Treap *Tr_Remove(Treap *treap, int key){
    Treap *smaller_eq, *larger, *target, *smaller;
    Tr_Split(treap, &smaller_eq, &larger, key);
    Tr_Split(smaller_eq, &smaller, &target, key - 1);
    Tr_DeleteNode(target);
    return Tr_Merge(smaller, larger);
}

/**
 * Prints directions to a node with a certain key in a Treap to stdout.
 * Directions are given starting from the root of the Treap. L means the child
 * node to the left of the current node, whereas R means the child node to the
 * right. -1 is returned in case no such node is found.
 * @param  treap Treap in which the search shall take place.
 * @param  key   Key of the element being searched for.
 */

void Tr_Locate(Treap *treap, int key){
    Treap *p = treap;
    // First pass to figure out if the node exists
    while (p && p->key != key)
        if (key > p->key)
            p = p->right;
        else
            p = p->left;
    if (!p)
        printf("-1"); // Doesn't exist
    else {
        // Exists; second pass to print directions
        p = treap;
        while (p->key != key)
            if (key > p->key){
                p = p->right;
                printf("R");
            } else {
                p = p->left;
                printf("L");
            }
    }
    printf("\n");
}

