#include "solver.h"

static int recursiveCall(int *cur, int *max, int sum, int limit_val){
    if (cur == max){
        return sum;
    }
    int add_val, sub_val;
    if (sum + *cur <= limit_val){
        add_val = recursiveCall(cur + 1, max, sum + *cur, limit_val);
    } else add_val = -1;
    if (sum - *cur >= 0){
        sub_val = recursiveCall(cur + 1, max, sum - *cur, limit_val);
    } else sub_val = -1;
    return sub_val >= add_val ? sub_val : add_val;
}

int findBestSolution(int *seq, int seq_len, int init_val, int limit_val){
    return recursiveCall(seq, seq + seq_len, init_val, limit_val);
}