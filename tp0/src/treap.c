#include "treap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define AUX_FLAG -1

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

Treap *Tr_Merge(Treap *smaller, Treap *larger){
    Treap *root = NULL; // Of the merged tree;
    Treap **branch = &root;
    while (smaller || larger){
        if (!smaller){
            *branch = larger;
            larger = NULL; // Finishes the procedure
        } else if (!larger) {
            *branch = smaller;
            smaller = NULL; // Finishes the procedure
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

