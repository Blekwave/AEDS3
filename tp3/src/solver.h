#ifndef _SOLVER_H_
#define _SOLVER_H_

#define DEFAULT_NUM_THREADS 1
extern int num_threads;

int findBestSolution(int *seq, int len, int init_val, int limit_val);

#endif