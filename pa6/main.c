#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define sq(X) ((X) * (X))

#define MAXTARGETS 500

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int distsq;
    int a;
    int b;
} Edge;

double distsq(Point a, Point b){
    return sq(a.x - b.x) + sq(a.y - b.y);
}

int compar(const void *a, const void *b){
    const Edge *A = a, *B = b;
    return A->distsq - B->distsq;
}

int main(int argc, char const *argv[]){
    int cases, prev_num_targets = 0;
    scanf("%d", &cases);
    Point *coords = NULL;
    bool *in_tree = NULL;
    Edge *edges = NULL;
    while (cases--){
        int num_targets, i, j, k, num_edges;
        scanf("%d", &num_targets);
        num_edges = (num_targets) * (num_targets - 1) / 2;
        if (num_targets > prev_num_targets){
            free(coords);
            free(in_tree);
            free(edges);
            coords = malloc(sizeof(Point) * num_targets);
            in_tree = malloc(sizeof(bool) * num_targets);
            edges = malloc(sizeof(Edge) * (num_targets) * (num_targets - 1) / 2);
        }
        for (i = 0, k = 0; i < num_targets; i++){
            scanf("%d %d", &coords[i].x, &coords[i].y);
            for (j = 0; j < i; j++, k++){
                edges[k].distsq = distsq(coords[i], coords[j]);
                edges[k].a = i;
                edges[k].b = j;
            }
            in_tree[i] = false;
        }
        if(num_edges != k){
            fprintf(stderr, "Mistakes were made\n");
            exit(1);
        }
        qsort(edges, (num_targets) * (num_targets - 1) / 2,
              sizeof(Edge), compar);
        double total_dist = 0;
        in_tree[0] = true;
        int nodes_in_tree;
        for (nodes_in_tree = 1; nodes_in_tree < num_targets; nodes_in_tree++){
            for (i = 0; i < num_edges; i++){
                if (in_tree[edges[i].a] ^ in_tree[edges[i].b])
                    break;
            }
            total_dist += sqrt(edges[i].distsq);
            in_tree[edges[i].a] = in_tree[edges[i].b] = true;
        }
        printf("%.2lf\n", total_dist / 100);
        prev_num_targets = num_targets;
    }
    free(coords);
    free(in_tree);
    free(edges);
}