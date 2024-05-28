#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "slow_functions.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int start_flag = 0;

void *slow_function1(void*);
void *slow_function2(void*);

void * call_slow_function1(void * arg){
    slow_function1(NULL);
    return NULL;
}

int main(){
    pthread_t threads[10];

    pthread_t slow_function2_thread;
    pthread_create(&slow_function2_thread, NULL, (void*)slow_function2, NULL);
    for (int i = 0; i< 10; i++){
        pthread_create(&threads[i], NULL, call_slow_function1, NULL);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Wait for slow_function2 to finish
    pthread_join(slow_function2_thread, NULL);

    return 0;
}

