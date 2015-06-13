#include "amgraph.h"
#include <stdlib.h>

/**
 * Initializes an undirected adjacency matrix graph.
 * @param  vertices Number of vertices in the graph.
 * @return          Address of the newly created graph.
 */
AMG *amgInit(int vertices){
    AMG *new = malloc(sizeof(AMG));
    new->edges = malloc(sizeof(AMGEdge) * vertices * vertices);
    new->size = vertices;
    return new;
}

/**
 * Sets an edge as existant and defines its weight.
 * @param amg    Graph.
 * @param a      One of the vertices of the edge.
 * @param b      The other vertex of the edge.
 * @param weight Weight of the edge. Irrelevant if the previous field is false.
 */
void amgSetEdge(AMG *amg, int a, int b, AMGWeight weight){
    amg->edges[a * amg->size + b] =
        amg->edges[b * amg->size + a] = (AMGEdge){weight, true};
}

/**
 * Returns the weight of an edge between A and B in a graph.
 * @param  amg Graph.
 * @param  a   Endpoint A of the edge.
 * @param  b   Endpoint B of the edge.
 * @return     Weight of the edge.
 */
AMGWeight amgGetWeight(AMG *amg, int a, int b){
    return (amg->edges[a * amg->size + b]).weight;
}

/**
 * Frees the memory associated with a graph.
 * @param amg Graph to be deleted.
 */
void amgDelete(AMG *amg){
    free(amg->edges);
    free(amg);
}