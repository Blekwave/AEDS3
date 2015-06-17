#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

int main(int argc, char const *argv[]){
    int cases;
    scanf("%d", &cases);
    while (cases--){
        int seq_len, init_val, limit_val, win_thres;
        scanf("%d %d %d %d", &seq_len, &init_val, &limit_val, &win_thres);
        int *seq = malloc(sizeof(int) * seq_len);
        int i;
        for (i = 0; i < seq_len; i++)
            scanf("%d", &seq[i]);
        
        for (i = 0; i < seq_len; i++)
            fprintf(stderr, "%d ", seq[i]);
        
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