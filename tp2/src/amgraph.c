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

AMG *amgCopy(AMG *original){
    AMG *new = amgInit(original->size);
    int i, j;
    for (i = 0; i < original->size; i++){
        for (j = 0; j < i; j++){
            new->edges[i][j] = new->edges[j][i] = original->edges[i][j];
        }
    }
    return new;
}

AMG *amgUnion(AMG *a, AMG *b){
    AMG *new = amgCopy(a);
    int i, j;
    for (i = 0; i < a->size; i++){
        for (j = 0; j < i; j++){
            if (b->edges[i][j].exists){
                new->edges[i][j] = new->edges[j][i] = b->edges[i][j];
            }
        }
    }
    return new;
}

void amgSetEdge(AMG *amg, int a, int b, bool exists, AMGWeight weight){
    amg->edges[a][b] = amg->edges[b][a] = (AMGEdge){weight, exists};
}

int amgGetSize(AMG *amg){
    return amg->size;
}

AMGWeight amgGetWeight(AMG *amg, int a, int b){
    return (amg->edges[a][b]).weight;
}

bool amgGetExistance(AMG *amg, int a, int b){
    return (amg->edges[a][b]).exists;
}

void amgDelete(AMG *amg){
    int i;
    for (i = 0; i < amg->size; i++){
        free(amg->edges[i]);
    }
    free(amg->edges);
    free(amg);
}