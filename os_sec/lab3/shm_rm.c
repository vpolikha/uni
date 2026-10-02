#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    for (int j = 1; j < argc; j++) {
        if (shmctl(atoi(argv[j]), IPC_RMID, NULL) == -1) {
            fprintf(stderr, "shmctl error");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
