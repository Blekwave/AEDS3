#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int dijkstra(int **graph, int num_cities, int orig, int dest){
    int *distances = malloc(sizeof(int) * num_cities);
    int i;
    for (i = 0; i < num_cities; i++)
        distances[i] = INT_MAX;
    bool *visited = calloc(sizeof(bool *), num_cities);

    int current = orig;
    visited[orig] = true;
    distances[orig] = 0;

    while (current != dest){
        for (i = 0; i < num_cities; i++){
            if (graph[current][i]){
                int new_distance;
                if (graph[i][current])
                    new_distance = distances[current];
                else
                    new_distance = distances[current] + graph[current][i];
                if (new_distance < distances[i])
                    distances[i] = new_distance;
            }
        }
        int candidate = -1;
        int candidate_dist = INT_MAX;
        for (i = 0; i < num_cities; i++)
            if (!visited[i] && distances[i] < candidate_dist){
                candidate = i;
                candidate_dist = distances[i];
            }
        if (candidate == -1)
            break;
        current = candidate;
        visited[current] = true;
    }

    int result = current == dest ? distances[current] : -1;
    free(distances);
    free(visited);
    return result;
}

int main(int argc, char const *argv[])
{
    int num_cities, num_agreements, i;
    while (scanf("%d %d", &num_cities, &num_agreements) != EOF 
           && (num_cities || num_agreements)){
        int **graph = malloc(sizeof(int *) * num_cities);
        for (i = 0; i < num_cities; i++)
            graph[i] = calloc(sizeof(int), num_cities);

        for (i = 0; i < num_agreements; i++){
            int x, y, h;
            scanf("%d %d %d", &x, &y, &h);
            graph[x - 1][y - 1] = h;
        }

        int num_queries;
        scanf("%d", &num_queries);

        for (i = 0; i < num_queries; i++){
            int orig, dest;
            scanf("%d %d", &orig, &dest);

            int delivery_time = dijkstra(graph, num_cities, orig - 1, dest - 1);
            if (delivery_time >= 0)
                printf("%d\n", delivery_time);
            else
                printf("Nao e possivel entregar a carta\n");
        }

        for (i = 0; i < num_cities; i++)
            free(graph[i]);
        free(graph);

        printf("\n"); // "Imprima uma linha em branco após cada caso de teste."
    }
    return 0;
}