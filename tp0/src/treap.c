#include "treap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define AUX_FLAG -1

///////////////////////////////////////
// Node and structure initialization //
///////////////////////////////////////

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

/**
 * Frees the memory for a single node.
 * @param treap Node to be deleted.
 */
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

///////////////////
// Tree rotation //
///////////////////

//////////////////////////////////////////////////////////////////////////////////
// Brief note about direction nomenclature: there is some controversy about the //
// meanings of rotating a tree right and left. In this program in particular,   //
// the direction chosen specifies the direction of the movement of the pivot's  //
// child element (B in the ASCII art demonstrations).                           //
//////////////////////////////////////////////////////////////////////////////////

/**
 * Performs a tree rotation to the right.
 * Example (R = root, P = pivot):
 * 
 *       R          P        
 *      / \        / \       
 *     P   C  ->  A   R  
 *    / \            / \     
 *   A   B          B   C    
 *
 * @param root  Root of the tree being rotated.
 * @param pivot Pivot element being brought up.
 */
void Tr_RotateRight(Treap *root, Treap *pivot){
    root->left = pivot->right;
    pivot->right = root;
}

/**
 * Performs a tree rotation to the left.
 * Example (R = root, P = pivot):
 * 
 *      R            P
 *     / \          / \
 *    A   P   ->   R   C
 *       / \      / \
 *      B   C    A   B
 *
 * @param root  Root of the tree being rotated.
 * @param pivot Pivot element being brought up.
 */
void Tr_RotateLeft(Treap *root, Treap *pivot){
    root->right = pivot->left;
    pivot->left = root;
}

/**
 * Inserts the larger Treap into the smaller Treap and fixes any heap property
 * violations that may occur.
 */
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


/**
 * Sets the Treap up for splitting by placing the element whose key is equal to
 * the split point at the root of the tree. If such element is non-existant, a
 * placeholder element is created and brought up.
 */
Treap *Tr_SplitSetup(Treap *root, Treap *root_parent, int split_point){
    if (root == NULL){
        // Node with split point as key not found.
        // New placeholder node is created with AUX_FLAG as its priority.
        // The node is not mistaken for a real node because priority values can
        // not be negative. (AUX_FLAG == -1)
        Treap *pivot = Tr_Init(split_point, AUX_FLAG);
        if (split_point > root_parent->key)
            root_parent->right = pivot;
        else
            root_parent->left = pivot;
        return pivot;
    }
    if (root->key == split_point) // Pivot found!
        return root;

    Treap *pivot;
    // Attempts to find the pivot recursively through the BST search algorithm
    if (split_point > root->key)
        pivot = Tr_SplitSetup(root->right, root, split_point);
    else
        pivot = Tr_SplitSetup(root->left, root, split_point);

    // Rotates the pivot up a level
    if (root->left == pivot)
        Tr_RotateRight(root, pivot);
    else
        Tr_RotateLeft(root, pivot);

    // Fixes the parent node to point to the correct node after rotation
    if (root_parent != NULL){
        if (root_parent->right == root)
            root_parent->right = pivot;
        else
            root_parent->left = pivot;
    }
    return pivot;
}

/**
 * Inserts the pivot of the splitting operation into the smaller or equal Treap.
 * Fixes any heap property violations by bringing the element up after that un-
 * til its parent's priority is smaller than its own. 
 * @param  root Root of the smaller Treap.
 * @param  node Pivot node of the splitting operation.
 * @return      Root of the Treap.
 */
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
            *smaller = Tr_SplitInsert(*smaller, source);
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

