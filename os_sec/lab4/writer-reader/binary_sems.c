#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>
#include "semun.h"
#include "binary_sems.h"

Boolean bsUseSemUndo = FALSE;

int initSemAvailable(int semId, int semNum) /* Initialize semaphore to 1 (i.e., "available") */
{
	union semun arg;
	arg.val = 1;

	return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum) /* Initialize semaphore to 0 (i.e., "in use") */
{
	union semun arg;
	arg.val = 0;

	return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum) /* Reserve semaphore - decrement it by 1 */
{
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = -1;
	sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

	return semop(semId, &sops, 1);
}

int releaseSem(int semId, int semNum) /* Release semaphore - increment it by 1 */
{
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = 1;
	sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

	return semop(semId, &sops, 1);
}
