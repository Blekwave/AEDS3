#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

int num_threads = DEFAULT_NUM_THREADS;

/**
 * The main function handles IO, calls the solver library and figures out
 * whether or not the player can win, based on the maximum valid amount of
 * points and on the win threshold.
 */
int main(int argc, char const *argv[]){
    if (argc == 2){
        num_threads = atoi(argv[1]);
    }
    int cases;
    scanf("%d", &cases);
    while (cases--){
        int seq_len, init_val, limit_val, win_thres;
        scanf("%d %d %d %d", &seq_len, &init_val, &limit_val, &win_thres);
        int *seq = malloc(sizeof(int) * seq_len);
        int i;
        for (i = 0; i < seq_len; i++)
            scanf("%d", &seq[i]);
        
        int res = findBestSolution(seq, seq_len, init_val, limit_val);
        if (res >= win_thres){
            printf("S,%d\n", res);
        } else {
            printf("N,%d\n", res);
        }
        
        free(seq);
    }
    return 0;
}