#ifndef _MST_H_
#define _MST_H_

#include "amgraph.h"

AMG *MST(AMG *graph);

double mstApproximation(AMG *graph, int *restrictions);

#endif