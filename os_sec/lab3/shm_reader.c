#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"

int main(int argc, char *argv[])
{
	int semid, shmid, transfers, bytes;
	struct shmseg *shmp;

	/* Get IDs for semaphore set and shared memory created by writer */
	semid = semget(SEM_KEY, 0, 0);

	if (semid == -1) {
		fprintf(stderr, "semget error");
		exit(EXIT_FAILURE);
	}

	shmid = shmget(SHM_KEY, 0, 0);

	if (shmid == -1) {
		fprintf(stderr, "shmget error");
		exit(EXIT_FAILURE);
	}

	shmp = shmat(shmid, NULL, SHM_RDONLY);

	if (shmp == (void *) -1) {
		fprintf(stderr, "shmat error");
		exit(EXIT_FAILURE);
	}

	/* Transfer blocks of data from shared memory to stdout */
	for (transfers = 0, bytes = 0; ; transfers++) {
		if (reserveSem(semid, READ_SEM) == -1) { /* Wait for our turn */
			fprintf(stderr, "reserveSem error");
			exit(EXIT_FAILURE);
		}

		if (shmp->cnt == 0) { /* Writer encountered EOF */
			break;
		}

		bytes += shmp->cnt;

		if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt) {
			fprintf(stderr, "partial/failed write");
			exit(EXIT_FAILURE);
		}

		if (releaseSem(semid, WRITE_SEM) == -1) { /* Give writer a turn */
			fprintf(stderr, "releaseSem error");
			exit(EXIT_FAILURE);
		}
	}

	if (shmdt(shmp) == -1) {
		fprintf(stderr, "shmdt error");
		exit(EXIT_FAILURE);
	}

	/* Give writer one more turn, so it can clean up */
	if (releaseSem(semid, WRITE_SEM) == -1) {
		fprintf(stderr, "releaseSem error");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "Received %d bytes (%d transfers)\n", bytes, transfers);
	exit(EXIT_SUCCESS);
}
