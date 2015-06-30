#include "solver.h"
#include <stdlib.h>
#include <stdbool.h>

int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    bool *prev = calloc(sizeof(bool), limit_val + 1);
    bool *cur = calloc(sizeof(bool), limit_val + 1);
    prev[init_val] = true;
    int i, j;
    for (i = 0; i < seq_len; i++){
        for (j = 0; j <= limit_val; j++){
            if (prev[j]){
                if (j + seq[i] <= limit_val)
                    cur[j + seq[i]] = true;
                if (j - seq[i] >= 0)
                    cur[j - seq[i]] = true;
            }
        }
        bool *swap = cur;
        cur = prev;
        prev = swap;
        for (j = 0; j <= limit_val; j++)
            cur[j] = false;
    }
    for (i = limit_val; i >= 0 && !prev[i]; i--);
    return i;
}