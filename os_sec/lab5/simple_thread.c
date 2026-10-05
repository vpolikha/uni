#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static void * threadFunc(void *arg)
{
    char *s = (char *) arg;
    printf("%s\n", s);
    size_t length = strlen(s);
    size_t *lengthP = malloc(sizeof(size_t));
    *lengthP = length;

    return (void *) lengthP;
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    void *res;
    int s;
    s = pthread_create(&t1, NULL, threadFunc, "Hello world");

    if (s != 0) {
        fprintf(stderr, "pthread_create error");
        exit(EXIT_FAILURE);
    }

    printf("Message from main()\n");
    s = pthread_join(t1, &res);

    if (s != 0) {
        fprintf(stderr, "pthread_join error");
        exit(EXIT_FAILURE);
    }

    printf("Thread returned %d\n", *(int*) res);
    free(res);
    exit(EXIT_SUCCESS);
}
