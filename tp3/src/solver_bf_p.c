#include "solver.h"
#include "queue.h"
#include <pthread.h>
#define NUM_THREADS 1

struct call {
    int depth;
    int sum;
};

struct thread_params {
    Queue *calls;
    int *seq;
    int len;
    int limit_val;
    int best_result;
    pthread_mutex_t *result_mutex;
};

static void *threadCall(void *data){
    struct thread_params *p = data;
    struct call call;
    int best_result = -1;
    while (1){
        if (qDequeue(p->calls, &call) != QUEUE_IS_EMPTY){
            if (call.sum + p->seq[call.depth] <= p->limit_val){
                if (call.depth + 1 == p->len){
                    if (call.sum > best_result){
                        best_result = call.sum + p->seq[call.depth];
                    }
                } else {
                    qEnqueue(p->calls, &(struct call)
                        {call.depth + 1, call.sum + p->seq[call.depth]});
                }
            }
            if (call.sum - p->seq[call.depth] >= 0){
                if (call.depth + 1 == p->len){
                    if (call.sum > best_result){
                        best_result = call.sum - p->seq[call.depth];
                    }
                } else {
                    qEnqueue(p->calls, &(struct call)
                        {call.depth + 1, call.sum - p->seq[call.depth]});
                }
            }
        } else { // Queue is empty
            break;
        }
    }
    pthread_mutex_lock(p->result_mutex);
    p->best_result = best_result;
    pthread_mutex_unlock(p->result_mutex);
    pthread_exit(NULL);
}

int findBestSolution(int *seq, int len, int init_val, int limit_val){
    pthread_t threads[NUM_THREADS];
    pthread_mutex_t result_mutex;
    pthread_mutex_init(&result_mutex, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    Queue *calls = qInit(sizeof(struct call));
    qEnqueue(calls, &(struct call){0, init_val});
    
    // Mandar threads consumirem da fila
    struct thread_params thread_params = {
        .calls = calls,
        .seq = seq,
        .len = len,
        .limit_val = limit_val,
        .best_result = -1,
        .result_mutex = &result_mutex
    };
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], &attr, threadCall, &thread_params);
    }
    for (int i = 0; i < NUM_THREADS; i++){
        void *status;
        pthread_join(threads[i], &status);
    }
    qDelete(calls);
    return thread_params.best_result;
}