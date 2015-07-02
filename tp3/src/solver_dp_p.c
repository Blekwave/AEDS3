#include "solver.h"
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

/**
 * Struct used to pass parameters to the thread call. Begin and end define the
 * range in the array where a given thread is supposed to work.
 */
struct args_next_state {
    int begin; // Inclusive
    int end; // Exclusive
    int item; // To be added or subtracted
    int limit_val;
    bool *prev;
    bool *cur;
};

/**
 * Auxiliary function to be called by pthread_create that computes the next
 * state of the game for a certain range of the previous state array. This
 * is done in a way that that all threads have an (almost) equal workload and
 * cover the entire array when combined (data parallelism).
 * @param  args Address to the argument struct (struct args_next_state).
 * @return      Irrelevant.
 */
void *processNextState(void *args){
    struct args_next_state *a = args;
    int i;
    for (i = a->begin; i < a->end; i++){
        if (a->prev[i]){ // If i is a sum attainable in the previous round.
            if (i + a->item <= a->limit_val)
                a->cur[i + a->item] = true;
            if (i - a->item >= 0)
                a->cur[i - a->item] = true;
        }
    }
    pthread_exit(NULL);
}

/**
 * Auxiliary function to be called by pthread_create that zeroes a certain range
 * of the previous state array. This range is defined by the parameters. This
 * function is used to zero the array in parallel (data parallelism).
 * @param  args Address to the argument struct (struct args_next_state).
 * @return      Irrelevant.
 */
void *zeroPrev(void *args){
    struct args_next_state *a = args;
    int i;
    for (i = a->begin; i < a->end; i++){
        a->prev[i] = false;
    }
    pthread_exit(NULL);
}

/**
 * Function that computes the best possible result in a game, based on the
 * starting value, the sequence of numbers and the upper limit.
 * @param  seq       Integer array which contains the numbers to be added or
 *                   subtracted.
 * @param  len       Length of the sequence.
 * @param  init_val  Initial value.
 * @param  limit_val Maximum value that can be reached without losing.
 * @return           Maximum amount of points that can be obtained by adding
 *                   or subtracting the numbers in the given sequence. If no
 *                   combination of plays can finish the game without breaking
 *                   the limit, -1 is returned.
 */
int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    pthread_t threads[num_threads];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Creates arrays of boolean values for the previous and current states of
    // the game.
    bool *prev = calloc(sizeof(bool), limit_val + 1);
    bool *cur = calloc(sizeof(bool), limit_val + 1);

    prev[init_val] = true; // Seeds the procedure by defining the initial value
                           // as a valid state in the previous round.

    struct args_next_state args[num_threads];
    int i, j;

    // Sets each thread's static parameters, including their activity ranges
    for (i = 0; i < num_threads; i++){
        args[i] = (struct args_next_state){
            .begin = (i * limit_val / num_threads),
            .end = ((i + 1) * limit_val / num_threads),
            .limit_val = limit_val
        };
    }
    args[num_threads - 1].end++; // Game state arrays have size limit_val + 1

    // Iterates through the sequence of numbers
    for (i = 0; i < seq_len; i++){
        for (j = 0; j < num_threads; j++){
            args[j].item = seq[i]; // Current item to be added or subtracted
            args[j].prev = prev; // Valid states for the previous round
            args[j].cur = cur; // Valid states for this round, to be found out
            pthread_create(&threads[j], &attr, processNextState, &args[j]);
        }
        for (j = 0; j < num_threads; j++){
            pthread_join(threads[j], NULL);
        }

        // Zeroes the previous round, which is going to be swapped and become
        // the next round.
        for (j = 0; j < num_threads; j++){
            pthread_create(&threads[j], &attr, zeroPrev, &args[j]);
        }
        for (j = 0; j < num_threads; j++){
            pthread_join(threads[j], NULL);
        }
        bool *swap = cur;
        cur = prev;
        prev = swap;
    }

    // Finds the maximum valid state after the final round by sweeping the array
    // from the end to the beginning. If there's no valid states, i finishes the
    // loop as -1, which is the desired value in this scenario.
    for (i = limit_val; i >= 0 && !prev[i]; i--);

    pthread_attr_destroy(&attr);
    return i;
}
