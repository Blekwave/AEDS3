#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int num_genes, num_reversals, num_queries, test_number = 1;
    int *reversals;
    int i, j;
    scanf("%d", &num_genes);
    while (num_genes){
        printf("Genome %d\n", test_number);

        scanf("%d", &num_reversals);
        reversals = malloc(sizeof(int) * num_reversals * 2);
        for (i = 0; i < num_reversals * 2; i += 2)
            scanf("%d %d", &reversals[i], &reversals[i + 1]);

        scanf("%d", &num_queries);
        for (i = 0; i < num_queries; i++){
            int query;
            scanf("%d", &query);
            for (j = 0; j < num_reversals * 2; j += 2)
                if (query >= reversals[j] && query <= reversals[j+1])
                    query = reversals[j + 1] - query + reversals[j];
            printf("%d\n", query);
        }

        test_number++;
        free(reversals);
        scanf("%d", &num_genes);
    }
    return 0;
}