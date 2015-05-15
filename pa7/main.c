#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_V 26
#define ASCII_SHIFT 97

typedef enum {NOT_VISITED, VISITED, SEARCHED, PRINTED} Status;

void breadth_first_search(bool graph[][MAX_V], int v, int origin, Status *status){
    int i;
    for (i = 0; i < v; i++){
        if (graph[origin][i] && status[i] == NOT_VISITED){
            status[i] = VISITED;
        }
    }
    for (i = 0; i < v; i++){
        if (graph[origin][i] && status[i] == VISITED){
            status[i] = SEARCHED;
            breadth_first_search(graph, v, i, status);
        }
    }
}

void find_connected_components(){
    int v, e;
    scanf("%d %d\n", &v, &e);
    bool graph[MAX_V][MAX_V];

    int i, j;
    for (i = 0; i < v; i++){
        for (j = 0; j < v; j++){
            graph[i][j] = false;
        }
    }

    for (i = 0; i < e; i++){
        char orig, dest;
        scanf("%c %c\n", &orig, &dest);
        graph[orig - ASCII_SHIFT][dest - ASCII_SHIFT] = 
            graph[dest - ASCII_SHIFT][orig - ASCII_SHIFT] = true;
    }

    Status status[MAX_V];
    for (i = 0; i < v; i++)
        status[i] = NOT_VISITED;

    int components = 0;

    for (i = 0; i < v; i++){
        if (status[i] == NOT_VISITED){
            breadth_first_search(graph, v, i, status);
            status[i] = SEARCHED;

            for (j = 0; j < v; j++){
                if (status[j] == SEARCHED){
                    printf("%c,", j + ASCII_SHIFT);
                    status[j] = PRINTED;
                }
            }

            i = 0;
            printf("\n");
            components++;
        }
    }
    printf("%d connected components\n\n", components);
}

int main(int argc, char const *argv[])
{
    int test_cases;
    scanf("%d", &test_cases);
    int i;
    for (i = 1; i <= test_cases; i++){
        printf("Case #%d:\n", i);
        find_connected_components();
    }
    return 0;
}