#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reversal(int *array, int lower, int higher){
    int stop = (higher - lower) / 2 + 1;
    int i;
    for (i = 0; i < stop; i++){
        int swap = array[lower + i];
        array[lower + i] = array[higher - i];
        array[higher - i] = swap;
    }
}

int main(int argc, char const *argv[])
{
    int num_genes, num_reversals, num_queries, test_number = 1;
    scanf("%d", &num_genes);
    while (num_genes){
        printf("Genome %d\n", test_number);
        int *genes = malloc(sizeof(int) * num_genes);
        genes--;
        int i;
        for (i = 1; i <= num_genes; i++)
            genes[i] = i;
        scanf("%d", &num_reversals);
        while (num_reversals--){
            int lower, higher;
            scanf("%d %d", &lower, &higher);
            reversal(genes, lower, higher);
        }
        scanf("%d", &num_queries);
        while (num_queries--){
            int query;
            scanf("%d", &query);
            for (i = 1; i <= num_genes; i++)
                if (genes[i] == query)
                    break;
            printf("%d\n", i);
        }
        test_number++;
        free(genes + 1);
        scanf("%d", &num_genes);
    }
    return 0;
}