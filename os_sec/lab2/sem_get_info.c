#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <time.h>
#include "sem.h"

int main(int argc, char *argv[])
{
    struct semid_ds ds;
    union semun arg, dummy; /* Fourth argument for semctl() */
    int semid, j;

    if (argc != 2) {
        fprintf(stderr, "Usage error: the program accepts exactly 1 argument - the semaphore set id\n");
        exit(EXIT_FAILURE);
    }

    semid = atoi(argv[1]);
    arg.buf = &ds;

    if (semctl(semid, 0, IPC_STAT, arg) == -1) {
        fprintf(stderr, "semctl error");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore changed: %s", ctime(&ds.sem_ctime));
    printf("Last semop(): %s", ctime(&ds.sem_otime));

    /* Display per-semaphore information */
    arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));

    if (arg.array == NULL) {
        fprintf(stderr, "calloc error");
        exit(EXIT_FAILURE);
    }

    if (semctl(semid, 0, GETALL, arg) == -1) {
        fprintf(stderr, "semctl-GETALL error");
        exit(EXIT_FAILURE);
    }

    printf("Sem # Value SEMPID SEMNCNT SEMZCNT\n");

    for (j = 0; j < ds.sem_nsems; j++) {
        printf(
            "%3d %5d %8d %5d %5d\n",
            j,
            arg.array[j],
            semctl(semid, j, GETPID, dummy),
            semctl(semid, j, GETNCNT, dummy),
            semctl(semid, j, GETZCNT, dummy)
        );
    }

    exit(EXIT_SUCCESS);
}
