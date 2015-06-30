#include "solver.h"
#include "queue.h"
#include <stdatomic.h>
#include <stdio.h>
#define NUM_THREADS 1

struct call {
    int depth;
    int sum;
};

struct thread_params {
    Queue *calls;
    pthread_mutex_t *result_mutex;
    pthread_mutex_t *sleep_mutex;
    pthread_cond_t *sleep_cond;
    _Atomic int *num_asleep;
    int *seq;
    int len;
    int limit_val;
    int best_result;
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
                    atomic_store(p->num_asleep, 0);
                    pthread_mutex_lock(p->sleep_mutex);
                    pthread_cond_broadcast(p->sleep_cond);
                    pthread_mutex_unlock(p->sleep_mutex);
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
                    atomic_store(p->num_asleep, 0);
                    pthread_mutex_lock(p->sleep_mutex);
                    pthread_cond_broadcast(p->sleep_cond);
                    pthread_mutex_unlock(p->sleep_mutex);
                }
            }
        } else { // Queue is empty
            int cur_asleep = atomic_fetch_add(p->num_asleep, 1) + 1;
            if (cur_asleep == NUM_THREADS){
                pthread_mutex_lock(p->sleep_mutex);
                pthread_cond_broadcast(p->sleep_cond);
                pthread_mutex_unlock(p->sleep_mutex);
                break;
            } else {
                pthread_mutex_lock(p->sleep_mutex);
                pthread_cond_wait(p->sleep_cond, p->sleep_mutex);
                pthread_mutex_unlock(p->sleep_mutex);
                cur_asleep = atomic_load(p->num_asleep);
                if (cur_asleep == NUM_THREADS){
                    break;
                }
            }
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
    pthread_mutex_t sleep_mutex;
    pthread_mutex_init(&sleep_mutex, NULL);
    pthread_cond_t sleep_cond;
    pthread_cond_init(&sleep_cond, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    Queue *calls = qInit(sizeof(struct call));
    qEnqueue(calls, &(struct call){0, init_val});
    
    _Atomic int num_asleep = 0;
    
    struct thread_params thread_params = {
        .calls = calls,
        .result_mutex = &result_mutex,
        .sleep_mutex = &sleep_mutex,
        .sleep_cond = &sleep_cond,
        .num_asleep = &num_asleep,
        .seq = seq,
        .len = len,
        .limit_val = limit_val,
        .best_result = -1
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