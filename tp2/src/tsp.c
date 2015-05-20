#include "tsp.h"
#include <stdlib.h>

#define sq(X) ((X) * (X))

long long distSquare(Point a, Point b){
    return sq(a.x - b.x) + sq(a.y - b.y);
}