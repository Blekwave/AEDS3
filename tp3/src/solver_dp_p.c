#include "solver.h"
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 2

struct args_next_state {
    int begin; // Inclusive
    int end; // Exclusive
    int item; // To be added or subtracted
    int limit_val;
    bool *prev;
    bool *cur;
};

void *processNextState(void *args){
    struct args_next_state *a = args;
    fprintf(stderr, "Alive! [%d, %d[ in %d\n", a->begin, a->end, a->item);
    int i;
    for (i = a->begin; i < a->end; i++){
        if (a->prev[i]){
            if (i + a->item <= a->limit_val)
                a->cur[i + a->item] = true;
            if (i - a->item >= 0)
                a->cur[i - a->item] = true;
        }
    }
    pthread_exit(NULL);
}

int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    bool *prev = calloc(sizeof(bool), limit_val + 1);
    bool *cur = calloc(sizeof(bool), limit_val + 1);
    prev[init_val] = true;
    int i, j;
    for (i = 0; i < seq_len; i++){
        for (j = 0; j < NUM_THREADS; j++){
            pthread_create(&threads[j], &attr, processNextState,
                &(struct args_next_state){
                    .begin = (j * limit_val / NUM_THREADS),
                    .end = ((j + 1) * limit_val / NUM_THREADS),
                    .item = seq[i],
                    .limit_val = limit_val,
                    .prev = prev,
                    .cur = cur
                });
        }
        for (j = 0; j < NUM_THREADS; j++){
            pthread_join(threads[i], NULL);
        }
        bool *swap = cur;
        cur = prev;
        prev = swap;
        for (j = 0; j <= limit_val; j++)
            cur[j] = false;
    }
    for (i = limit_val; i >= 0 && !prev[i]; i--);
    
    pthread_attr_destroy(&attr);
    return i;
}