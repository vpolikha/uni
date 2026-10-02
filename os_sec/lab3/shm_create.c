#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int segSize = atoi(argv[1]);
    int shmid = shmget(IPC_PRIVATE, segSize, S_IRUSR | S_IWUSR);

    if (shmid == -1) {
        fprintf(stderr, "shmget");
        exit(EXIT_FAILURE);
    }

    printf("%d\n", shmid);      /* On success, display shared memory ID */
    exit(EXIT_SUCCESS);
}
