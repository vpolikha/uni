#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shm.h"

int main(int argc, char *argv[])
{
    int semid, fd, transfers, bytes;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 0, 0);

    if (semid == -1) {
        fprintf(stderr, "semget error\n");
        exit(EXIT_FAILURE);
    }

    fd = open("shared.dat", O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "open error\n");
        exit(EXIT_FAILURE);
    }

    shmp = mmap(NULL, sizeof(struct shmseg),
                PROT_READ,
                MAP_SHARED, fd, 0);

    if (shmp == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(EXIT_FAILURE);
    }

    close(fd);

    for (transfers = 0, bytes = 0; ; transfers++) {

        if (reserveSem(semid, READ_SEM) == -1) {
            fprintf(stderr, "reserveSem error\n");
            exit(EXIT_FAILURE);
        }
        if (shmp->cnt == 0) {
            break;
        }

        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        if (releaseSem(semid, WRITE_SEM) == -1) {
            fprintf(stderr, "releaseSem error\n");
            exit(EXIT_FAILURE);
        }
    }

    if (munmap(shmp, sizeof(struct shmseg)) == -1) {
        fprintf(stderr, "munmap error\n");
        exit(EXIT_FAILURE);
    }

    if (releaseSem(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "releaseSem error\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Received %d bytes (%d transfers)\n",
            bytes, transfers);

    exit(EXIT_SUCCESS);
}
