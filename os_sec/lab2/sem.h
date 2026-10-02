#ifndef SEM_H
#define SEM_H /* Prevent accidental double inclusion */
#include <sys/sem.h>

union semun { /* Used in calls to semctl() */
    int val; /* Value for SETVAL */
    struct semid_ds * buf; /* Buffer for IPC_STAT, IPC_SET */
    unsigned short * array; /* Array for GETALL, SETALL */
};

char * currTime(const char *format);

#endif
