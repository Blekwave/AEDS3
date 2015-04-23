#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reversal(int *genes, int num_genes, int lower, int higher){
    int i;
    for (i = 1; i <= num_genes; i++){
        if (genes[i] && genes[i] >= lower && genes[i] <= higher)
            genes[i] = higher - genes[i] + lower;
    }
}

int main(int argc, char const *argv[])
{
    int num_genes, num_reversals, num_queries, test_number = 1;
    scanf("%d", &num_genes);
    while (num_genes){
        printf("Genome %d\n", test_number);
        
        int *genes = calloc(sizeof(int), num_genes);
        genes--;

        scanf("%d", &num_reversals);
        int *reversals = malloc(sizeof(int) * num_reversals * 2);
        int i = 0;
        while (i < (num_reversals * 2)){
            scanf("%d %d", &reversals[i], &reversals[i + 1]);
            i += 2;
        }

        scanf("%d", &num_queries);
        int *queries = malloc(sizeof(int) * num_queries);
        i = 0;
        while (i < num_queries){
            int query;
            scanf("%d", &query);
            genes[query] = query;
            queries[i] = query;
            i++;
        }

        for (i = 0; i < num_reversals * 2; i += 2){
            reversal(genes, num_genes, reversals[i], reversals[i + 1]);
        }

        for (i = 0; i < num_queries; i++)
            printf("%d\n", genes[queries[i]]);

        free(reversals);
        free(genes + 1);
        test_number++;
        scanf("%d", &num_genes);
    }
    return 0;
}