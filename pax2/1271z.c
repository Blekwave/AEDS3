#include <stdio.h>
#include <stdlib.h>

void quick_read(int *acc){
    *acc = 0;
    register int c = getchar();
    while (c > 47 && c < 58){
        *acc = (*acc << 1) + (*acc << 3) + c - '0';
        c = getchar();
    }
}

void quick_read_us(unsigned short *acc){
    *acc = 0;
    register int c = getchar();
    while (c > 47 && c < 58){
        *acc = (*acc << 1) + (*acc << 3) + c - '0';
        c = getchar();
    }
}

void query_processing(int num_queries, unsigned short *reversals, int num_reversals){
    int i, j;
    for (i = 0; i < num_queries; i++){
        int query;
        quick_read(&query);
        for (j = 0; j < num_reversals; j += 2){
            if (query >= reversals[j] && query <= reversals[j + 1])
                query = reversals[j + 1] - query + reversals[j];
        }
        printf("%d\n", query);
    }
}

int main(int argc, char const *argv[])
{
    int num_genes, num_reversals = 0, num_queries, test_number = 1;
    unsigned short *reversals = NULL;
    quick_read(&num_genes);
    while (num_genes){
        printf("Genome %d\n", test_number);

        int nrev_prev = num_reversals;
        quick_read(&num_reversals);
        num_reversals <<= 1;

        if (num_reversals > nrev_prev){
            free(reversals);
            reversals = malloc(sizeof(unsigned short) * num_reversals);
        }
        

        int i = 0;
        while (i < num_reversals){
            quick_read_us(&reversals[i]);
            i++;
        }

        quick_read(&num_queries);
        query_processing(num_queries, reversals, num_reversals);

        test_number++;
        
        quick_read(&num_genes);
    }
    free(reversals);
    return 0;
}