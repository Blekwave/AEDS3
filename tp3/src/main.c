#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

#define MEASURE_EXECUTION_TIME_POSIX

#ifdef MEASURE_EXECUTION_TIME_POSIX
    #include <time.h>
#endif

int num_threads = DEFAULT_NUM_THREADS;

/**
 * The main function handles IO, calls the solver library and figures out
 * whether or not the player can win, based on the maximum valid amount of
 * points and on the win threshold.
 */
int main(int argc, char const *argv[]){
    #ifdef MEASURE_EXECUTION_TIME_POSIX
        struct timespec begin, end;   
        clock_gettime(CLOCK_MONOTONIC, &begin);
    #endif

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

    #ifdef MEASURE_EXECUTION_TIME_POSIX
        clock_gettime(CLOCK_MONOTONIC, &end);

        double execution_time = (end.tv_sec - begin.tv_sec);
        execution_time += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;

        fprintf(stderr, "%lf", execution_time);
    #endif

    return 0;
}