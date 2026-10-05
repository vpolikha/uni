#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *doubleNumber(void *arg)
{
    int *num = arg;

    int *result = malloc(sizeof(int));
    if (result == NULL)
        exit(EXIT_FAILURE);

    *result = (*num) * 2;

    return result;
}

static void *tripleNumber(void *arg)
{
    int *num = arg;

    int *result = malloc(sizeof(int));
    if (result == NULL)
        exit(EXIT_FAILURE);

    *result = (*num) * 3;

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s number\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int number = atoi(argv[1]);

    pthread_t t1, t2;
    void *res;

    pthread_create(&t1, NULL, doubleNumber, &number);
    pthread_join(t1, &res);

    int doubled = *(int *)res;
    free(res);

    pthread_create(&t2, NULL, tripleNumber, &doubled);
    pthread_join(t2, &res);

    int final = *(int *)res;
    free(res);

    printf("Result = %d\n", final);

    return 0;
}