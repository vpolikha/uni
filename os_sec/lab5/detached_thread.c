#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

static void * threadFunc(void *arg)
{
    // pthread_detach(pthread_self()); /* Call if thread is not created with DETACHED attribute */
    char *s = (char *) arg;
    printf("%s\n", s);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_attr_t attr;
    int s;

    s = pthread_attr_init(&attr); /* Assigns default values */

    if (s != 0) {
        fprintf(stderr, "pthread_attr_init error");
        exit(EXIT_FAILURE);
    }

    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (s != 0) {
        fprintf(stderr, "pthread_attr_setdetachstate error");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t1, &attr, threadFunc, "Hello world");

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    // pthread_detach(t1); /* Call if thread is not created with DETACHED attribute */

    printf("Message from main()\n");
    s = pthread_attr_destroy(&attr); /* No longer needed */

    if (s != 0) {
        fprintf(stderr, "pthread_attr_destroy error");
        exit(EXIT_FAILURE);
    }

    sleep(1); /* Give some time for the thread function to execute */
    exit(EXIT_SUCCESS);
}
