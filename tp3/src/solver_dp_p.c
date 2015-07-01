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

    struct args_next_state args[NUM_THREADS];
    int i, j;
    for (i = 0; i < NUM_THREADS; i++){
        args[i] = (struct args_next_state){
            .begin = (i * limit_val / NUM_THREADS),
            .end = ((i + 1) * limit_val / NUM_THREADS),
            .limit_val = limit_val
        };
    }
    args[NUM_THREADS - 1].end++; // Game state arrays have size limit_val + 1
    for (i = 0; i < seq_len; i++){
        for (j = 0; j < NUM_THREADS; j++){
            args[j].item = seq[i];
            args[j].prev = prev;
            args[j].cur = cur;
            pthread_create(&threads[j], &attr, processNextState, &args[j]);
        }
        for (j = 0; j < NUM_THREADS; j++){
            pthread_join(threads[j], NULL);
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
