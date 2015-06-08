#include "amgraph.h"
#include <stdlib.h>

AMG *amgInit(int vertices){
    AMG *new = malloc(sizeof(AMG));
    new->edges = malloc(sizeof(AMGEdge *) * vertices);
    int i;
    for (i = 0; i < vertices; i++){
        new->edges[i] = malloc(sizeof(AMGEdge) * vertices);
    }
    new->size = vertices;
    return new;
}

void amgSetEdge(AMG *amg, int a, int b, bool exists, AMGWeight weight){
    amg->edges[a][b] = amg->edges[b][a] = (AMGEdge){weight, exists};
}

AMGWeight amgGetWeight(AMG *amg, int a, int b){
    return (amg->edges[a][b]).weight;
}

void amgDelete(AMG *amg){
    int i;
    for (i = 0; i < amg->size; i++){
        free(amg->edges[i]);
    }
    free(amg->edges);
    free(amg);
}