#ifndef _TSP_H_
#define _TSP_H_

typedef struct {
    int x;
    int y;
} Point;

double findMinDistance(Point *coords, int num_cities,
                       int *restrictions, int *num_restrictions);

#endif