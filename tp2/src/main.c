#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tsp.h"

int main(int argc, char const *argv[]){
    int num_cities;
    int i;
    while (scanf("%d", &num_cities) != EOF){
        Point *coords = malloc(sizeof(Point) * num_cities);
        for (i = 0; i < num_cities; i++){
            scanf("%d %d", &coords[i].x, &coords[i].y);
        }
        
        int *restrictions = calloc(sizeof(int), num_cities);
        
        int num_restrictions;
        scanf("%d", &num_restrictions);
        
        for (i = 0; i < num_restrictions; i++){
            int first, second;
            scanf("%d %d", &first, &second);
            restrictions[second] |= 1 << first;
            // Second should not be visited before first
        }

        double distance = findMinDistance(coords, num_cities, restrictions);

        if (distance >= 0){
            printf("%.2lf\n", distance);
        } else {
            printf("Deadlock\n");
        }
        
        free(restrictions);
        free(coords);
    }
    return 0;
}