#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

static void *sumFirst(void *arg)
{
    int *result = malloc(sizeof(int));

    if (result == NULL)
        exit(EXIT_FAILURE);

    *result = 0;

    for (int i = 0; i < 5; i++)
        *result += data[i];

    return result;
}

static void *sumSecond(void *arg)
{
    int *result = malloc(sizeof(int));

    if (result == NULL)
        exit(EXIT_FAILURE);

    *result = 0;

    for (int i = 5; i < 10; i++)
        *result += data[i];

    return result;
}

int main(void)
{
    pthread_t t1, t2;
    void *res1;
    void *res2;

    if (pthread_create(&t1, NULL, sumFirst, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&t2, NULL, sumSecond, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(t1, &res1) != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(t2, &res2) != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    int sum1 = *(int *)res1;
    int sum2 = *(int *)res2;

    free(res1);
    free(res2);

    printf("First half: %d\n", sum1);
    printf("Second half: %d\n", sum2);
    printf("Total: %d\n", sum1 + sum2);

    return 0;
}