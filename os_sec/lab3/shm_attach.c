#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    printf("SHMLBA = %ld (%#lx), PID = %ld\n",
            (long) SHMLBA, (unsigned long) SHMLBA, (long) getpid());

    for (int j = 1; j < argc; j++) {
        char *p;
        int shmid = strtol(argv[j], NULL, 0);

        char *retAddr = shmat(shmid, NULL, 0);
        if (retAddr == (void *) -1) {
            fprintf(stderr, "shmat: %s", argv[j]);
            exit(EXIT_FAILURE);
        }

        printf("%d: %s ==> %p\n", j, argv[j], retAddr);
    }

    printf("Sleeping 5 seconds\n");
    sleep(5);

    exit(EXIT_SUCCESS);
}
