#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_t t1;

static void * threadFunc(void *arg)
{
    if (pthread_equal(t1, pthread_self())) {
        printf("t1 matches with thread id\n");
    } else {
        printf("t1 doesn't match with thread id\n");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int s;
    s = pthread_create(&t1, NULL, threadFunc, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    printf("Message from main()\n");
    s = pthread_join(t1, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
