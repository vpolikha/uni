#ifndef BINARY_SEMS_H /* Prevent accidental double inclusion */
#define BINARY_SEMS_H

typedef enum { FALSE, TRUE } Boolean;
extern Boolean bsUseSemUndo; /* Use SEM_UNDO during semop()? */

int initSemAvailable(int semId, int semNum);
int initSemInUse(int semId, int semNum);
int reserveSem(int semId, int semNum);
int releaseSem(int semId, int semNum);

#endif
