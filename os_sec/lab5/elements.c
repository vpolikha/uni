#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "get_element.h"

static void * threadFunc(void *arg)
{
    char *el;
    printf("Other thread about to call getElement()\n");
    el = getElement(1);
    printf("Other thread: element = %s\n", el);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *el;
    el = getElement(0);
    printf("Main thread has called getElement()\n");
    s = pthread_create(&t, NULL, threadFunc, NULL);

    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t, NULL);

    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    printf("Main thread: element = %s\n", el);
    exit(EXIT_SUCCESS);
}
