#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "unix_stream.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sfd == -1) {
        fprintf(stderr, "socket error");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "connect error");
        exit(EXIT_FAILURE);
    }

    /* Copy stdin to socket */

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(sfd, buf, numRead) != numRead) {
            fprintf(stderr, "partial/failed write");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        fprintf(stderr, "read error");
        exit(EXIT_FAILURE);
    }

    while ((numRead = read(sfd, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            fprintf(stderr, "write error\n");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        fprintf(stderr, "rwrite error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);         /* Closes our socket; server sees EOF */
}
