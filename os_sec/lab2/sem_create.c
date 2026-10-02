#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include "sem.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage error: the program accepts exactly one init argument.");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    int semid = semget(IPC_PRIVATE, 4, S_IRUSR | S_IWUSR);

    if (semid == -1) {
        fprintf(stderr, "semid error");
        exit(EXIT_FAILURE);
    }

    arg.val = atoi(argv[1]);

    if (semctl(semid, /* semnum= */ 0, SETVAL, arg) == -1) {
        fprintf(stderr, "semid error");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore ID = %d\n", semid);
    exit(EXIT_SUCCESS);
}
