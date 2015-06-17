#include "solver.h"

static int recursiveCall(int *cur, int *max, int sum, int limit_val){
    if (cur == max){
        return sum;
    }
    int sum_val, return_val;
    if (sum + *cur <= limit_val){
        sum_val = recursiveCall(cur + 1, max, sum + *cur, limit_val);
    } else sum_val = -1;
    if (sum - *cur >= 0){
        return_val = recursiveCall(cur + 1, max, sum - *cur, limit_val);
    } else return_val = -1;
    return return_val >= sum_val ? return_val : sum_val;
}

int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    return recursiveCall(seq, seq + seq_len, init_val, limit_val);
}