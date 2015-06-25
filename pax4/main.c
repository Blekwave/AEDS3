#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int partialMinMax(int *sum, int *prev, int cut, int num_panels){
    int i, local_max, min_max = INT_MAX;
    for (i = cut; i <= num_panels; i++){
        int local_sum = sum[i] - sum[cut];
        local_max = local_sum > prev[i] ? local_sum : prev[i];
        if (local_max < min_max)
            min_max = local_max;
    }
    return min_max;
}

int main(){
    int cases;
    scanf("%d", &cases);
    while (cases--){
        int num_panels, num_trucks, shipping;
        scanf("%d %d %d", &num_panels, &num_trucks, &shipping);
        // Shipping rate in dollars per truck per kg
        int i, j, k, panel;
        int *sum = calloc(sizeof(int), num_panels + 1);
        for (i = 1; i <= num_panels; i++){
            scanf("%d", &panel);
            sum[i] = sum[i - 1] + panel;
        }
        int max_val;
        if (num_trucks == 1){
            max_val = sum[num_panels];
        } else {
            int *prev = calloc(sizeof(int), num_panels + 1);
            int *cur = calloc(sizeof(int), num_panels + 1);
            
            // Fill line where p = 1 (sum of all panels - partial sums)
            for (i = 0; i <= num_panels; i++){
                prev[i] = sum[num_panels] - sum[i];
            }
            
            // Compute minimum maximum partitioning for each #partitions and
            // each sub-array (cutting from the left).
            for (i = 1; i < num_trucks - 1; i++){
                for (j = 0; j <= num_panels; j++){
                    cur[j] = partialMinMax(sum, prev, j, num_panels);
                }
                int *swap = cur;
                cur = prev;
                prev = swap;
            }
            
            max_val = partialMinMax(sum, prev, 0, num_panels);
            
            free(prev);
            free(cur);
        }
        printf("%d $%d\n", max_val, max_val * num_trucks * shipping);
        free(sum);
    }
    return 0;
}