#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static void *threadFunc(void *arg)
{
    char *str = (char *)arg;
    size_t *length = malloc(sizeof(size_t));

    if (length == NULL) {
        exit(EXIT_FAILURE);
    }

    *length = strlen(str);

    return length;
}

int main(void)
{
    pthread_t thread;
    void *result;
    char *str = "Hello world";
    int s = pthread_create(&thread, NULL, threadFunc, str);

    if (s != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(thread, &result);
    if (s != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    printf("Length = %zu\n", *(size_t *)result);

    free(result);
    return 0;
}