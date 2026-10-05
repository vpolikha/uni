#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int data[10];

static void *fillEven(void *arg)
{
    for (int i = 0; i < 10; i += 2)
        data[i] = i;

    return NULL;
}

int main(void)
{
    pthread_t t1;

    for (int i = 1; i < 10; i += 2)
        data[i] = i;

    if (pthread_create(&t1, NULL, fillEven, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(t1, NULL) != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++)
        printf("data[%d] = %d\n", i, data[i]);

    return 0;
}