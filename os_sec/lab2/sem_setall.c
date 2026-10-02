#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "sem.h" /* Definition of semun union */

int main(int argc, char *argv[])
{
	struct semid_ds ds;
	union semun arg; /* Fourth argument for semctl() */
	int j, semid;

	if (argc < 3) {
		fprintf(stderr, "%s semid val...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	semid = atoi(argv[1]);
	/* Obtain size of semaphore set */
	arg.buf = &ds;

	if (semctl(semid, 0, IPC_STAT, arg) == -1) {
        fprintf(stderr, "semctl\n");
		exit(EXIT_FAILURE);
	}

	if (ds.sem_nsems != argc - 2) {
		fprintf(
			stderr,
			"Set contains %ld semaphores, but %d values were supplied\n",
			(long) ds.sem_nsems,
			argc - 2
		);
        exit(EXIT_FAILURE);
	}

	/* Set up array of values; perform semaphore initialization */
	arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));

	if (arg.array == NULL) {
		fprintf(stderr, "calloc\n");
		exit(EXIT_FAILURE);
	}

	for (j = 2; j < argc; j++) {
		arg.array[j - 2] = atoi(argv[j]);
	}

	if (semctl(semid, 0, SETALL, arg) == -1) {
		fprintf(stderr, "semctl-SETALL\n");
		exit(EXIT_FAILURE);
	}

	printf("Semaphore values changed (PID=%ld)\n", (long) getpid());
	exit(EXIT_SUCCESS);
}
