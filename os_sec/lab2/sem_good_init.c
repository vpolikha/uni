#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include "sem.h"

int main(int argc, char *argv[])
{
    int key = 15;
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	if (semid != -1) { /* Successfully created the semaphore */
        printf(
            "Process ID: %d\n"
            "Successfully created the semaphore set.\n\n",
            getpid()
        );

		union semun arg;
		struct sembuf sop;
		arg.val = 0; /* So initialize it to 0 */

		if (semctl(semid, 0, SETVAL, arg) == -1) {
			fprintf(stderr, "semctl");
			exit(EXIT_FAILURE);
		}

		/* Perform a "no-op" semaphore operation - changes sem_otime
		so other processes can see we've initialized the set. */
		sop.sem_num = 0; /* Operate on semaphore 0 */
		sop.sem_op = 0; /* Wait for value to equal 0 */
		sop.sem_flg = 0;

		if (semop(semid, &sop, 1) == -1) {
			fprintf(stderr, "semop");
			exit(EXIT_FAILURE);
		}

        printf(
            "Process ID: %d\n"
            "Performing a 'no-op' semaphore operation to change sem_otime field.\n\n",
            getpid()
        );
	} else { /* We didn't create the semaphore set */
        printf(
            "Process ID: %d\n"
            "Semaphore set already created by another process.\n\n",
            getpid()
        );

		const int MAX_TRIES = 10;
		int j;
		union semun arg;
		struct semid_ds ds;

		if (errno != EEXIST) { /* Unexpected error from semget() */
			fprintf(stderr, "semget");
			exit(EXIT_FAILURE);
		}

		semid = semget(key, 1, S_IRUSR | S_IWUSR); /* Retrieve ID of existing set */

		if (semid == -1) {
			fprintf(stderr, "semget");
			exit(EXIT_FAILURE);
		}

		/* Wait until another process has called semop() */
		arg.buf = &ds;

		for (j = 0; j < MAX_TRIES; j++) {
			if (semctl(semid, 0, IPC_STAT, arg) == -1) {
				fprintf(stderr, "semctl");
				exit(EXIT_FAILURE);
			}

			if (ds.sem_otime != 0) { /* semop() performed? */
                printf(
                    "Process ID: %d\n"
                    "semop() performed by the other process. Operations on semaphore set are safe now.\n\n",
                    getpid()
                );
				break; /* Yes, quit loop */
			}

			sleep(1); /* If not, wait and retry */
		}

		if (ds.sem_otime == 0) { /* Loop ran to completion! */
			fprintf(stderr, "Existing semaphore not initialized");
			exit(EXIT_FAILURE);
		}
	}
}
