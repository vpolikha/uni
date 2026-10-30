#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int src_fd, dest_fd;
    struct stat sb;
    char *src_addr, *dest_addr;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s src dest\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    src_fd = open(argv[1], O_RDONLY);

    if (src_fd == -1) {
        perror("Source file");
        exit(EXIT_FAILURE);
    }

    if (fstat(src_fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    dest_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (dest_fd == -1) {
        perror("Destination file");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(dest_fd, sb.st_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    src_addr = mmap(NULL, sb.st_size,
                    PROT_READ, MAP_PRIVATE,
                    src_fd, 0);

    if (src_addr == MAP_FAILED) {
        perror("mmap source");
        exit(EXIT_FAILURE);
    }

    dest_addr = mmap(NULL, sb.st_size,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     dest_fd, 0);

    if (dest_addr == MAP_FAILED) {
        perror("mmap destination");
        exit(EXIT_FAILURE);
    }

    memcpy(dest_addr, src_addr, sb.st_size);

    if (msync(dest_addr, sb.st_size, MS_SYNC) == -1) {
        perror("msync");
        exit(EXIT_FAILURE);
    }

    munmap(src_addr, sb.st_size);
    munmap(dest_addr, sb.st_size);

    close(src_fd);
    close(dest_fd);

    return 0;
}
