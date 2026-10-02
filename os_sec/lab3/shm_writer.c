#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semun.h"
#include "shm.h"

int main(int argc, char *argv[])
{
	int semid, shmid, bytes, transfers;
	struct shmseg *shmp;
	union semun dummy;
	semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);

	if (semid == -1) {
		fprintf(stderr, "semget error");
		exit(EXIT_FAILURE);
	}

	if (initSemAvailable(semid, WRITE_SEM) == -1) {
		fprintf(stderr, "initSemAvailable error");
		exit(EXIT_FAILURE);
	}

	if (initSemInUse(semid, READ_SEM) == -1) {
		fprintf(stderr, "initSemInUse error");
		exit(EXIT_FAILURE);
	}

	shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);

	if (shmid == -1) {
		fprintf(stderr, "shmget error");
		exit(EXIT_FAILURE);
	}

	shmp = shmat(shmid, NULL, 0);

	if (shmp == (void *) -1) {
		fprintf(stderr, "shmat error");
		exit(EXIT_FAILURE);
	}

	/* Transfer blocks of data from stdin to shared memory */
	for (transfers = 0, bytes = 0; ; transfers++, bytes += shmp->cnt) {
		if (reserveSem(semid, WRITE_SEM) == -1) { /* Wait for our turn */
			fprintf(stderr, "reserveSem error");
			exit(EXIT_FAILURE);
		}

		shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);

		if (shmp->cnt == -1) {
			fprintf(stderr, "read error");
			exit(EXIT_FAILURE);
		}

		if (releaseSem(semid, READ_SEM) == -1) { /* Give reader a turn */
			fprintf(stderr, "releaseSem error");
			exit(EXIT_FAILURE);
		}

		/* Have we reached EOF? We test this after giving the reader
		a turn so that it can see the 0 value in shmp->cnt. */
		if (shmp->cnt == 0) {
			break;
		}
	}

	/* Wait until reader has let us have one more turn. We then know
	reader has finished, and so we can delete the IPC objects. */
	if (reserveSem(semid, WRITE_SEM) == -1) {
		fprintf(stderr, "reserveSem error");
		exit(EXIT_FAILURE);
	}

	if (semctl(semid, 0, IPC_RMID, dummy) == -1) {
		fprintf(stderr, "semctl error");
		exit(EXIT_FAILURE);
	}

	if (shmdt(shmp) == -1) {
		fprintf(stderr, "shmdt error");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl error");
		exit(EXIT_FAILURE);
	}

	printf("Sent %d bytes (%d transfers)\n", bytes, transfers);
	exit(EXIT_SUCCESS);
}
