#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include "sem.h"

int main(int argc, char *argv[])
{
    int semid;

    if (argc != 3) {
        fprintf(
            stderr,
            "Usage error: the program accepts exactly 2 arguments in the following format:\n"
            "%s sem_id operation\n",
            argv[0]
        );
        exit(EXIT_FAILURE);
    }

    struct sembuf sop; /* Structure defining operation */
    semid = atoi(argv[1]);
    sop.sem_num = 0; /* Specifies first semaphore in set */
    sop.sem_op = atoi(argv[2]); /* Add, subtract, or wait for 0 */
    sop.sem_flg = 0; /* No special options for operation */

    printf("%ld: about to semop at %s\n", (long) getpid(), currTime("%T"));

    if (semop(semid, &sop, 1) == -1) {
        fprintf(stderr, "semop error");
        exit(EXIT_FAILURE);
    }

    printf("%ld: semop completed at %s\n", (long) getpid(), currTime("%T"));
    exit(EXIT_SUCCESS);
}
