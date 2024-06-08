#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "slow_functions.h"

// Initialize a mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize a condition variable for thread synchronization
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Flag to indicate the start of processing
int start_flag = 0;

// Function prototypes
void *slow_function1(void*);
void *slow_function2(void*);

// Wrapper function to call slow_function1
void * call_slow_function1(void * arg){
    slow_function1(NULL);
    return NULL;
}

int main(){
    pthread_t threads[10]; // Array to hold thread IDs for 10 threads

    pthread_t slow_function2_thread; // Thread ID for slow_function2

    // Create a thread to run slow_function2
    pthread_create(&slow_function2_thread, NULL, (void*)slow_function2, NULL);

    // Create 10 threads to run slow_function1
    for (int i = 0; i < 10; i++){
        pthread_create(&threads[i], NULL, call_slow_function1, NULL);
    }

    // Wait for all 10 threads to finish execution
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Wait for slow_function2 to finish
    pthread_join(slow_function2_thread, NULL);

    return 0;
}
