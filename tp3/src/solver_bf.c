#include "solver.h"

/**
 * Recursive call used to find out the best possible result via brute force.
 * @param seq       Integer array which contains the numbers to be added or
 *                  subtracted.
 * @param pos       Current item to be added or subtracted.
 * @param len       Length of the sequence.
 * @param sum       Sum to the current state.
 * @param limit_val Maximum value that can be reached without losing.
 * @param best      Best end-game value found so far.
 */
static void recursiveCall(int *seq, int pos, int len,
                         int sum, int limit_val, int *best){
    if (sum + seq[pos] <= limit_val){
        if (pos + 1 == len){
            if (sum + seq[pos] > *best){
                *best = sum + seq[pos];
            }
        } else {
            recursiveCall(seq, pos + 1, len,
                          sum + seq[pos], limit_val, best);
        }
    }
    if (sum - seq[pos] >= 0){
        if (pos + 1 == len){
            if (sum - seq[pos] > *best){
                *best = sum - seq[pos];
            }
        } else {
            recursiveCall(seq, pos + 1, len,
                          sum - seq[pos], limit_val, best);
        }
    }
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
int findBestSolution(int *seq, int len, int init_val, int limit_val){
    int best = -1;
    recursiveCall(seq, 0, len, init_val, limit_val, &best);
    return best;
}