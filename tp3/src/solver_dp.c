#include "solver.h"
#include <stdlib.h>
#include <stdbool.h>

#define NO_FALLBACK -1

bool fallback(int *alt, int *value, int *depth){
    while (*depth >= 0 && alt[*depth] == NO_FALLBACK)
        (*depth)--;
    if (*depth >= 0){
        *value = alt[*depth];
        alt[*depth] = NO_FALLBACK;
        return true;
    } else {
        return false;
    }
}

int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    int *alt = malloc(sizeof(int) * seq_len);
    int i;
    for (i = 0; i < seq_len; i++){
        alt[i] = NO_FALLBACK;
    }
    int value = init_val;
    int max_finish = -1;
    int depth = 0;
    while (true){
        if (depth == seq_len){
            if (value > max_finish)
                max_finish = value;
            depth--;
            if (!fallback(alt, &value, &depth))
                break;
        } else if (value + seq[depth] <= limit_val){
            if (value - seq[depth] >= 0){
                alt[depth] = value - seq[depth];
            }
            value += seq[depth];
        } else if (value - seq[depth] >= 0){
            value -= seq[depth];
        } else {
            if (!fallback(alt, &value, &depth))
                break;
        }
        depth++;
    }
    free(alt);
    return max_finish;
}