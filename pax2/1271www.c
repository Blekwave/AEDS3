#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char const *argv[])
{
    int num_genes, num_reversals, num_queries, test_number = 1;
    scanf("%d", &num_genes);
    while (num_genes){
        printf("Genome %d\n", test_number);
 
        scanf("%d", &num_reversals);
        int *reversals = malloc(sizeof(int) * num_reversals * 2);
        int i = 0;
        while (i < (num_reversals * 2)){
            scanf("%d %d", &reversals[i], &reversals[i + 1]);
            i += 2;
        }
 
        scanf("%d", &num_queries);
        int *queries = malloc(sizeof(int) * num_queries);
        for (i = 0; i < num_queries; i++)
            scanf("%d", &queries[i]);
 
        int j;
        for (i = 0; i < num_reversals * 2; i += 2)
            for (j = 0; j < num_queries; j++)
                if (queries[j] && queries[j] >= reversals[i] && queries[j] <= reversals[i+1])
                    queries[j] = reversals[i + 1] - queries[j] + reversals[i];
 
        for (i = 0; i < num_queries; i++)
            printf("%d\n", queries[i]);
 
        free(reversals);
        free(queries);
 
        test_number++;
        scanf("%d", &num_genes);
    }
    return 0;
}