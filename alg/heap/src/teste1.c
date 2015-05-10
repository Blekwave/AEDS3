#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define iv(X) (*(int *)(X))

long long compar(const void *a, const void *b){
    return iv(a) - iv(b);
}

int main(int argc, char const *argv[])
{
    Heap *heap = hInit(sizeof(int), 30, compar);

    // Test #1: Single insertion
    printf("Test #1: single insertion\n");
    int a = 12;
    hInsert(heap, &a);
    assert(heap->num == 1);
    printf("Number of elements correct.\n");
    assert(iv(heap->root + heap->size) == a);
    printf("Value of single element correct.\n");
    int temp;
    hPop(heap, &temp);
    assert(temp == a);
    printf("Removal of element yielded appropriate results.\n");



    int valores[9] = {4, 17, 31, -3, 2, -20, 0, 0, 0};
    int i, min = 2000000;
    for (i = 0; i < 9; i++){
        hInsert(heap, &valores[i]);
        if (valores[i] < min)
            min = valores[i];
        assert(min == iv(heap->root + heap->size));
        printf("Root check #%d (%d) correct (r = %d).\n", i, valores[i], min);
    }

    for (i = 0; i < 9; i++){
        hPop(heap, &temp);
        printf("%d\n", temp);
    }
    hDelete(heap);
    return 0;
}