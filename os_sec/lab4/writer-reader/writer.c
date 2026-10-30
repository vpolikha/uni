#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "semun.h"
#include "shm.h"

int main(int argc, char *argv[])
{
    int semid, fd, bytes, transfers;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);

    if (semid == -1) {
        fprintf(stderr, "semget error\n");
        exit(EXIT_FAILURE);
    }

    if (initSemAvailable(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "initSemAvailable error\n");
        exit(EXIT_FAILURE);
    }

    if (initSemInUse(semid, READ_SEM) == -1) {
        fprintf(stderr, "initSemInUse error\n");
        exit(EXIT_FAILURE);
    }

    fd = open("shared.dat", O_RDWR | O_CREAT, 0666);

    if (fd == -1) {
        fprintf(stderr, "open error\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(struct shmseg)) == -1) {
        fprintf(stderr, "ftruncate error\n");
        exit(EXIT_FAILURE);
    }

    shmp = mmap(NULL, sizeof(struct shmseg),
                PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);

    if (shmp == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(EXIT_FAILURE);
    }

    close(fd);

    for (transfers = 0, bytes = 0; ; transfers++, bytes += shmp->cnt) {

        if (reserveSem(semid, WRITE_SEM) == -1) {
            fprintf(stderr, "reserveSem error\n");
            exit(EXIT_FAILURE);
        }

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);

        if (shmp->cnt == -1) {
            fprintf(stderr, "read error\n");
            exit(EXIT_FAILURE);
        }

        if (releaseSem(semid, READ_SEM) == -1) {
            fprintf(stderr, "releaseSem error\n");
            exit(EXIT_FAILURE);
        }

        if (shmp->cnt == 0) {
            break;
        }
    }

    if (reserveSem(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "reserveSem error\n");
        exit(EXIT_FAILURE);
    }

    if (semctl(semid, 0, IPC_RMID, 0) == -1) {
        fprintf(stderr, "semctl error\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(shmp, sizeof(struct shmseg)) == -1) {
        fprintf(stderr, "munmap error\n");
        exit(EXIT_FAILURE);
    }

    printf("Sent %d bytes (%d transfers)\n", bytes, transfers);

    exit(EXIT_SUCCESS);
}