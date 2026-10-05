#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "curr_time.h"
#define ARR_SIZE 5

static int arr[ARR_SIZE];
static int available = 0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc()
{
    printf("Thread 1 starting at %s\n", currTime("%T"));
    int s;
    s = pthread_mutex_lock(&threadMutex);

    if (s != 0) {
        fprintf(stderr, "pthread_mutex_lock error");
        exit(EXIT_FAILURE);
    }

    available++;
    s = pthread_mutex_unlock(&threadMutex);

    if (s != 0) {
        fprintf(stderr, "pthread_mutex_unlock error");
        exit(EXIT_FAILURE);
    }

    pthread_cond_signal(&cond);
    sleep(2);
    printf("Thread 1 terminating at %s\n", currTime("%T"));
    return 0;
}

int main(int argc, char *argv[])
{
    printf("Main thread starting at %s\n", currTime("%T"));
    pthread_t t1;
    void *res;
    int s;
    s = pthread_create(&t1, NULL, threadFunc, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t1, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_join error");
        exit(EXIT_FAILURE);
    }

    s = pthread_mutex_lock(&threadMutex);

    if (s != 0) {
        fprintf(stderr, "pthread_mutex_lock error");
        exit(EXIT_FAILURE);
    }

    // s = pthread_cond_wait(&cond, &threadMutex);

    // if (s != 0) {
    //     fprintf(stderr, "pthread_cond_wait error");
    //     exit(EXIT_FAILURE);
    // }

    available--;
    s = pthread_mutex_unlock(&threadMutex);

    if (s != 0) {
        fprintf(stderr, "pthread_mutex_unlock error");
        exit(EXIT_FAILURE);
    }

    sleep(3);
    printf("Main thread terminating at %s\n", currTime("%T"));
    exit(EXIT_SUCCESS);
}
