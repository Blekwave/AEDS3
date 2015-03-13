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

/**
 * Performs a tree rotation between a parent and one of its child nodes.
 * @param root  Treap parent node to be rotated.
 * @param pivot Treap child node to be rotated.
 */
void Tr_Rotate(Treap *root, Treap *pivot){
    // !!Provide pictures!!
    // Transfer one of the pivot's children to the root
    if (root->left == pivot){
        root->left = pivot->right;
        pivot->right = root;
    } else {
        root->right = pivot->left;
        pivot->left = root;
    }
    // Does not set root's parent's child pointer to pivot!
}

/**
 * Searches for an element with a certain key and makes it the root element of
 * the Treap through recursive rotations.
 * @param  root Root of the subtree to be replaced.
 * @param  key  Key of the element to be brought to the top.
 * @return      Address of the new root element.
 */
Treap *Tr_BringToRoot(Treap *root, Treap *root_parent, int key){
    // O(log n)
    if (root == NULL || root->key == key)
        return root;

    Treap *pivot;
    if (key > root->key)
        pivot = Tr_BringToRoot(root->right, root, key);
    else
        pivot = Tr_BringToRoot(root->left, root, key);

    if (pivot != NULL){
        Tr_Rotate(root, pivot);
        if (root_parent != NULL){
            if (root_parent->right == root)
                root_parent->right = pivot;
            else
                root_parent->left = pivot;
        }
    }
    return pivot;
}

/**
 * Recursively determines the element with the least priority in the Treap.
 * @param root Root of the treap to be evaluated.
 * @param min  Address to an integer where the minimum priority will be stored.
 * @param key  Address to an integer where the minimum element's key will be
 *             stored.
 */
void Tr_MinPriority(Treap *root, int *min, int *key){
    // O(n)
    if (root->left != NULL)
        Tr_MinPriority(root->left, min, key);
    if (root->right != NULL)
        Tr_MinPriority(root->right, min, key);
    if (root->pri < *min){
        *min = root->pri;
        *key = root->key;
    }
}

/**
 * Corrects a Treap's root to make sure it fulfills the heap property: the root
 * must have the least priority of any of its children.
 * @param  root        Root of the treap (or sub-treap).
 * @param  root_parent Parent of the root (or NULL)
 * @return             Address of the root of the treap after corrections.
 */
Treap *Tr_MakeHeap(Treap *root, Treap *root_parent){
    // O(n²)
    if (root != NULL){
        int min = INT_MAX, key = -1;
        Tr_MinPriority(root, &min, &key);
        root = Tr_BringToRoot(root, root_parent, key);
        if (root->left != NULL)
            root->left = Tr_MakeHeap(root->left, root);
        if (root->right != NULL)
            root->right = Tr_MakeHeap(root->right, root);
    }
    return root;
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
        Tr_Rotate(smaller, larger);
        if (sm_parent != NULL)
            sm_parent->right = larger;
        subtree_root = larger;
        sm_parent = larger;
        larger = smaller->right;
    }
    while (larger != NULL && smaller->pri > larger->pri){
        Tr_Rotate(smaller, larger);
        sm_parent->left = larger;
        sm_parent = larger;
        larger = smaller->right;
    }
    return subtree_root;
}

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
        if (root_parent != NULL){
            if (key > root_parent->key)
                root_parent->right = pivot;
            else
                root_parent->left = pivot;
        }
        return pivot;
    }
    if (root->key == key)
        return root;

    Treap *pivot;
    if (key > root->key)
        pivot = Tr_SplitSetup(root->right, root, key);
    else
        pivot = Tr_SplitSetup(root->left, root, key);

    Tr_Rotate(root, pivot);
    if (root_parent != NULL){
        if (root_parent->right == root)
            root_parent->right = pivot;
        else
            root_parent->left = pivot;
    }
    return pivot;
}

void Tr_Split(Treap *source, Treap **smaller, Treap **larger, int split_point){
    source = Tr_SplitSetup(source, NULL, split_point);
    if (source->pri == AUX_FLAG){
        // Split point was not the key of an existing element.
        // An auxiliary node was created to make the process possible.
        *smaller = source->left;
        *larger = source->right;
        Tr_DeleteNode(source);
    } else {
        // The root of the source tree has split point as its key
        *smaller = source;
        *larger = source->right;
        (*smaller)->right = NULL;
    }

    // Enforces heap properties
    if (*smaller != NULL)
        *smaller = Tr_MakeHeap(*smaller, NULL);
    if (*larger != NULL)
        *larger = Tr_MakeHeap(*larger, NULL);
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

