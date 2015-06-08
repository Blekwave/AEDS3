#ifndef _AMGRAPH_H_
#define _AMGRAPH_H_

#include <stdbool.h>

typedef double AMGWeight;

typedef struct {
    AMGWeight weight;
    bool exists;
} AMGEdge;

typedef struct {
    AMGEdge **edges;
    int size;
} AMG;

AMG *amgInit(int vertices);

void amgSetEdge(AMG *amg, int a, int b, bool exists, AMGWeight weight);

AMGWeight amgGetWeight(AMG *amg, int a, int b);

void amgDelete(AMG *amg);

#endif