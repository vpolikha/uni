#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include "sem.h"

int main(int argc, char *argv[])
{
    int semid;

    if (argc != 2) {
        fprintf(
            stderr,
            "Usage error: the program accepts exactly 1 argument in the following format:\n"
            "%s sem_id\n",
            argv[0]
        );
        exit(EXIT_FAILURE);
    }

    struct sembuf sop; /* Structure defining operation */
    semid = atoi(argv[1]);
    sop.sem_num = 0; /* Specifies first semaphore in set */
    sop.sem_op = 2; /* Add 2 to the value */
    sop.sem_flg = 0; /* Undo the operation after process termination */

    if (semop(semid, &sop, 1) == -1) {
        fprintf(stderr, "semop error");
        exit(EXIT_FAILURE);
    }

    printf("semop() permformed successfully.\n");
}
