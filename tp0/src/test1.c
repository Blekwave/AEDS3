#include <stdio.h>
#include <stdlib.h>
#include "treap.h"

int main(int argc, char const *argv[])
{
    // Test #1: Init
    Treap *init = Tr_Init(3,1);
    if (init && init->key == 3 && init->pri == 1)
        printf("Test #1 (Init): Pass\n");
    else {
        printf("Test #1 (Init): Fail (Aborting execution)\n");
        exit(1);
    }

    // Test #2: Insert
    init = Tr_Insert(init, 2, 6);
    init = Tr_Insert(init, 1, 4);
    if (init->left->key == 1 && init->left->right->key == 2)
        printf("Test #2 (Insert): Pass\n");
    else {
        printf("Test #2 (Insert): Fail (Aborting execution)\n");
        exit(2);
    }

    // Test #3: Remove
    init = Tr_Remove(init, 3);
    if (init && init->key == 1 && init->right->key == 2)
        printf("Test #3 (Remove): Pass\n");
    else {
        printf("Test #3 (Remove): Fail (Aborting execution)\n");
        exit(2);
    }

    // Test #-1: Delete
    Tr_Delete(init);
    printf("Test -1 (Delete): Pass\n");
    
    return 0;
}
