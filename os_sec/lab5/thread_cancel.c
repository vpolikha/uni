#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

static void * threadFunc(void *arg)
{
    printf("New thread started\n");

    for (int i = 0; ; i++)
    {
        printf("%d\n", i);
        sleep(1); /* A cancellation point */
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    printf("Main thread started\n");
    pthread_t t1;
    void *res;
    int s;
    s = pthread_create(&t1, NULL, threadFunc, "Hello world");

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    sleep(3);
    s = pthread_cancel(t1);

    if (s != 0) {
        fprintf(stderr, "pthread_cancel error");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t1, &res);

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    if (res == PTHREAD_CANCELED) {
        printf("Thread was canceled\n");
    }

    exit(EXIT_SUCCESS);
}
