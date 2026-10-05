#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;
static void *threadFunc(void *arg)
{
    int loops = *(int*) arg;
    int loc, j;

    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;
    loops = (argc > 1) ? atoi(argv[1]) : 10000000;
    int *loopsP = malloc(sizeof(int));
    *loopsP = loops;

    s = pthread_create(&t1, NULL, threadFunc, loopsP);

    if (s != 0) {
        fprintf(stderr, "pthread_create");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t2, NULL, threadFunc, loopsP);

    if (s != 0) {
        fprintf(stderr, "pthread_create");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t1, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_join");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t2, NULL);

    if (s != 0) {
        fprintf(stderr, "pthread_join");
        exit(EXIT_FAILURE);
    }

    printf("glob = %d\n", glob);
    free(loopsP);
    exit(EXIT_SUCCESS);
}
