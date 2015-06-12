#include "mst.h"
#include <stdlib.h>
#include <float.h>

typedef struct {
    double weight;
    int a;
    int b;
} MSTEdge;

static int mstEdgeCompar(const void *a, const void *b){
    const MSTEdge *A = a, *B = b;
    return A->weight - B->weight;
}

AMG *MST(AMG *graph){
    int size = amgGetSize(graph);
    MSTEdge *edges = malloc(sizeof(MSTEdge) * size * (size - 1) / 2);
    int i, j, num_edges = 0;
    for (i = 0; i < size; i++){
        for (j = 0; j < i; j++){
            if (amgGetExistance(graph, i, j)){
                edges[num_edges] =
                    ((MSTEdge) {amgGetWeight(graph, i, j), i, j});
                num_edges++;
            }
        }
    }
    qsort(edges, num_edges, sizeof(MSTEdge), mstEdgeCompar);
    
    AMG *tree = amgInit(size);
    
    bool *in_tree = calloc(sizeof(bool), size);
    int num_in_tree = 1;
    in_tree[0] = true;
    
    for (i = 0; i < num_edges && num_in_tree < size; i++){
        if (in_tree[edges[i].a] ^ in_tree[edges[i].b]){
            amgSetEdge(tree, edges[i].a, edges[i].b, true, edges[i].weight);
            in_tree[edges[i].a] = in_tree[edges[i].b] = true;
            i = 0;
            num_in_tree++;
        }
    }
    
    free(in_tree);
    free(edges);
    
    return tree;
}

static void recursiveCall(AMG *graph, AMG *mst, double *dist, int current,
                   int *previous, int *visited, int *restrictions,
                   bool *success){
    if ((restrictions[current] & *visited) != restrictions[current]){
        *visited = -1;
        *success = false;
        return;
    }
    *dist += amgGetWeight(graph, *previous, current);
    *visited |= 1 << current;
    *previous = current;
    int i;
    for (i = 0; i < amgGetSize(mst); i++){
        if (!((*visited >> i) & 1) && amgGetExistance(mst, current, i)){
            recursiveCall(graph, mst, dist, i, previous, visited, restrictions,
                          success);
        }
    }
};

double preOrderDistance(AMG *mst, AMG *graph, int *restrictions){
    double dist = 0;
    int previous = 0;
    int visited = 1; // bit mask (1: 0 has been visited)
    bool success = true;
    
    int i;
    for (i = 0; i < amgGetSize(mst); i++){
        if (!((visited >> i) & 1) && amgGetExistance(mst, 0, i)){
            recursiveCall(graph, mst, &dist, i, &previous, &visited, restrictions,
                          &success);
        }
    }
    
    return success ? dist : DBL_MAX;
}

double mstApproximation(AMG *graph, int *restrictions){
    AMG *mst = MST(graph);
    return preOrderDistance(mst, graph, restrictions);
}