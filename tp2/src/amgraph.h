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

AMG *amgCopy(AMG *original);

AMG *amgUnion(AMG *a, AMG *b);

void amgSetEdge(AMG *amg, int a, int b, bool exists, AMGWeight weight);

int amgGetSize(AMG *amg);

AMGWeight amgGetWeight(AMG *amg, int a, int b);

bool amgGetExistance(AMG *amg, int a, int b);

void amgDelete(AMG *amg);

#endif