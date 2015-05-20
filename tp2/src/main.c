#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "tsp.h"

int main(){
    int num_cities;
    int i;
    while (scanf("%d", &num_cities) != EOF){
        Point *coords = malloc(sizeof(Point) * num_cities);
        for (i = 0; i < num_cities; i++){
            scanf("%d %d", &coords[i].x, &coords[i].y);
        }
        
        bool *restrictions = calloc(sizeof(bool), num_cities * num_cities);
        
        int num_restrictions;
        scanf("%d", &num_restrictions);
        
        for (i = 0; i < num_restrictions; i++){
            int first, second;
            scanf("%d %d", &first, &second);
            restrictions[second][first] = true;
            // Second should not be visited before first
        }
        
        free(restrictions);
        free(coords);
    }
}