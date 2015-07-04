#include "solver.h"
#include <stdlib.h>
#include <stdbool.h>

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
    // Creates arrays of boolean values for the previous and current states of
    // the game.
    bool *prev = calloc(sizeof(bool), limit_val + 1);
    bool *cur = calloc(sizeof(bool), limit_val + 1);

    prev[init_val] = true; // Seeds the procedure by defining the initial value
                           // as a valid state in the previous round.

    int i, j;

    // Iterates through the sequence of numbers
    for (i = 0; i < seq_len; i++){
        for (j = 0; j <= limit_val; j++){
            if (prev[j]){ // If j is a sum attainable in the previous round.
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

    free(prev);
    free(cur);
    return i;
}