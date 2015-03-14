#include "treap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define AUX_FLAG -1

///////////////////////////////////////
// Node and structure initialization //
///////////////////////////////////////

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

void Tr_Delete(Treap *treap){
    if (treap == NULL)
        return;
    Tr_Delete(treap->left);
    Tr_Delete(treap->right);
    Tr_DeleteNode(treap);
}

void Tr_RotateRight(Treap *root, Treap *pivot){
    root->left = pivot->right;
    pivot->right = root;
}

void Tr_RotateLeft(Treap *root, Treap *pivot){
    root->right = pivot->left;
    pivot->left = root;
}

Treap *Tr_MergeInsert(Treap *smaller, Treap *larger, Treap *sm_parent){
    if (smaller->right == NULL){
        smaller->right = larger;
    }
    else {
        larger = Tr_MergeInsert(smaller->right, larger, smaller);
    }

    Treap *subtree_root = smaller;
    if (smaller->pri > larger->pri){
        // Performs a rotation to correct possible violations of the heap pro-
        // perty because of the implantation of larger into smaller.
        Tr_RotateLeft(smaller, larger);
        if (sm_parent != NULL)
            sm_parent->right = larger;
        subtree_root = larger;
        sm_parent = larger;
        larger = smaller->right;
    }
    while (larger != NULL && smaller->pri > larger->pri){
        // Performs further rotations to correct possible violations of the
        // heap property because of the previous rotation.
        Tr_RotateLeft(smaller, larger);
        sm_parent->left = larger;
        sm_parent = larger;
        larger = smaller->right;
    }
    return subtree_root;
}

/**
 * Merges two Treaps A and B, where the keys of all elements of A are smaller
 * than the ones in the elements of B.
 * @param  smaller   Treap with the smaller elements
 * @param  larger    Treap with the larger elements
 * @return           Address of the root element of the merged Treaps.
 */
Treap *Tr_Merge(Treap *smaller, Treap *larger){
    if (smaller == NULL)
        return larger;
    if (larger == NULL)
        return smaller;

    return Tr_MergeInsert(smaller, larger, NULL);
}

Treap *Tr_SplitSetup(Treap *root, Treap *root_parent, int key){
    if (root == NULL){
        Treap *pivot = Tr_Init(key, AUX_FLAG); // -1 pri flags node as auxiliary
        if (key > root_parent->key)
            root_parent->right = pivot;
        else
            root_parent->left = pivot;
        return pivot;
    }
    if (root->key == key)
        return root;

    Treap *pivot;
    if (key > root->key)
        pivot = Tr_SplitSetup(root->right, root, key);
    else
        pivot = Tr_SplitSetup(root->left, root, key);

    if (root->left == pivot)
        Tr_RotateRight(root, pivot);
    else
        Tr_RotateLeft(root, pivot);

    if (root_parent != NULL){
        if (root_parent->right == root)
            root_parent->right = pivot;
        else
            root_parent->left = pivot;
    }
    return pivot;
}

Treap *Tr_SplitInsert(Treap *root, Treap *node){
    if (root == NULL)
        return node;
    if (node->key > root->key){
        if (root->right == NULL){
            root->right = node;
            if (root->pri > node->pri){
                Tr_RotateLeft(root, node);
                return node;
            }
            else
                return root;
        }
        else {
            Treap *source_next = Tr_SplitInsert(root->right, node);
            if (source_next == node && root->pri > node->pri){
                Tr_RotateLeft(root, node);
                return node;
            }
            else
                return root;
        }
    } else {
        if (root->left == NULL){
            root->left = node;
            if (root->pri > node->pri){
                Tr_RotateRight(root, node);
                return node;
            }
            else
                return root;
        }
        else {
            Treap *source_next = Tr_SplitInsert(root->left, node);
            if (source_next == node && root->pri > node->pri){
                Tr_RotateRight(root, node);
                return node;
            }
            else
                return root;
        }
    }
}

void Tr_Split(Treap *source, Treap **smaller, Treap **larger, int split_point){
    if (source != NULL){
        source = Tr_SplitSetup(source, NULL, split_point);
        *smaller = source->left;
        *larger = source->right;
        if (source->pri == AUX_FLAG){
            // Split point was not the key of an existing element.
            // An auxiliary node was created to make the process possible.
            Tr_DeleteNode(source);
        } else {
            // The root of the source tree has split point as its key
            source->right = source->left = NULL;
            Tr_SplitInsert(*smaller, source);
        }
    }
}

Treap *Tr_Insert(Treap *treap, int key, int pri){
    Treap *new = Tr_Init(key, pri);
    Treap *smaller = NULL, *larger = NULL;
    Tr_Split(treap, &smaller, &larger, key);
    return Tr_Merge(Tr_Merge(smaller, new), larger);
}

Treap *Tr_Remove(Treap *treap, int key){
    Treap *smaller_eq = NULL, *larger = NULL, *target = NULL, *smaller = NULL;
    Tr_Split(treap, &smaller_eq, &larger, key);
    Tr_Split(smaller_eq, &smaller, &target, key - 1);
    Tr_DeleteNode(target);
    return Tr_Merge(smaller, larger);
}

bool Tr_Locate(Treap *treap, int key, char *directions, int position,
    int *maxdepth){
    if (treap == NULL){
        return false;
    }
    if (treap->key == key){
        *maxdepth = position;
        return true;
    }
    if (key > treap->key){
        if (Tr_Locate(treap->right, key, directions, position + 1, maxdepth)){
            directions[position] = 'R';
            return true;
        }
    } else {
        if (Tr_Locate(treap->left, key, directions, position + 1, maxdepth)){
            directions[position] = 'L';
            return true;
        }
    }
    return false;
}

