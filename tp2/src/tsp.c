#include "tsp.h"
#include "mst.h"
#include "amgraph.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <stdio.h>

#define sq(X) ((X) * (X))

/**
 * Calculates the Euclidean distance between two 2D points.
 * @param  a Point A
 * @param  b Point B
 * @return   Distance between A and B.
 */
double dist(Point a, Point b){
    return sqrt(sq((long long)a.x - b.x) + sq((long long)a.y - b.y));
}

/**
 * Creates an adjacency matrix graph in which the edges represent the distance
 * between two cities.
 * @param  coords     Coordinates of each city.
 * @param  num_cities Number of cities.
 * @return            Address to the newly created graph (must be deleted later)
 */
AMG *buildDistanceGraph(Point *coords, int num_cities){
    AMG *graph = amgInit(num_cities);
    int i, j;
    for (i = 0; i < num_cities; i++){
        for (j = 0; j < i; j++){
            amgSetEdge(graph, i, j, true, dist(coords[i], coords[j]));
        }
    }
    return graph;
}

/**
 * Recursively determines the shortest path through brute force.
 * @param graph        Graph in which the edges represent the distance between
 *                     two cities.
 * @param in_path      Bit mask which represents which cities have already been
 *                     visited.
 * @param num_cities   Total number of cities.
 * @param restrictions Restriction array (explained in findMinDistance's doc)
 * @param distance     Length of the current path.
 * @param current_city Last city added to the path, current location.
 * @param min_dist     Minimum distance found so far, kept globally between
 *                     calls (its address is passed).
 */
static void recursiveCall(AMG *graph, int in_path, int num_cities, 
                          int *restrictions, double distance,
                          int current_city, double *min_dist){
    // If the path is valid and complete, it will correspond of a bit string of
    // num_cities 1s. (1 << num_cities) - 1 is a way of creating such a string,
    // in order to check if the path is finished.
    if (in_path == (1 << num_cities) - 1){
        // Going back to the original city is required.
        distance += amgGetWeight(graph, current_city, 0);
        if (distance < *min_dist){
            *min_dist = distance;
        }
        return;
    }
    
    // Brute force attempt at reaching the optimal solution
    int i;
    for (i = 1; i < num_cities; i++){
        // !(1 & (in_path >> i)): checks if city #i has already been visited.
        // (restrictions[i] & in_path) == restrictions[i]: checks if all restri-
        // ctions for accessing city #i have been accomplished.
        if (!(1 & (in_path >> i)) &&
            (restrictions[i] & in_path) == restrictions[i]){
            double new_dist = distance + amgGetWeight(graph, current_city, i);
            // If the length of the path is greater than the minimum distance
            // found so far, there's no point in proceeding, since it can only
            // get lengthier.
            if (new_dist < *min_dist){
                // Calls recursiveCall once again, adding city #i to the current
                // path and updating the distance accordingly.
                recursiveCall(graph, in_path | (1 << i), num_cities, 
                              restrictions, new_dist, i, min_dist);
            }
        }
    }
}

/**
 * Given an array of cities' coordinates and an array of restrictions, this
 * function determines the shortest path, starting from city #0, to visit each
 * city and go back to city #0. The restrictions determine which cities must 
 * have already been visited before going to a certain city.
 * @param  coords       Array of the cities' 2D coordinates.
 * @param  num_cities   Number of cities.
 * @param  restrictions Array of access restrictions for each city. These res-
 *                      trictions are represented through bit masks.
 *                      E.g.: restrictions[3] = 1100111
 *                      The above restriction mask means that city #3 can only
 *                      be accessed if cities #0, #1, #2, #5 and #6 have already
 *                      been visited.
 * @return              Length of the shortest path.
 */
double findMinDistance(Point *coords, int num_cities,
                       int *restrictions, int *num_restrictions){
    if (num_cities == 1)
        return 0;

    AMG *graph = buildDistanceGraph(coords, num_cities);


    double min_dist = mstApproximation(graph, restrictions);
    

    if (*num_restrictions == 0){
        *num_restrictions = 1;
        restrictions[2] = 1 << 1; // 2 can only be accessed if 1 has already
                                  // been visited.
    }

    recursiveCall(graph, 1, num_cities, restrictions, 0, 0, &min_dist);

    amgDelete(graph);

    if (min_dist == DBL_MAX)
        return -1;
    return min_dist;
}