#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "unix_stream.h"
#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        fprintf(stderr, "socket error\n");
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        fprintf(stderr, "remove-%s error", SV_SOCK_PATH);
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        if (errno == EADDRINUSE) {
            fprintf(stderr, "Address already in use\n");
        } else {
            fprintf(stderr, "bind error\n");
        }

        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1) {
        fprintf(stderr, "listen error\n");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        cfd = accept(sfd, NULL, NULL);

        if (cfd == -1) {
            fprintf(stderr, "accept error\n");
            exit(EXIT_FAILURE);
        }

        /* Transfer data from connected socket to stdout until EOF */

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            if (write(cfd, buf, numRead) != numRead) {
                fprintf(stderr, "partial/failed write\n");
                exit(EXIT_FAILURE);
            }
        }

        if (numRead == -1) {
            fprintf(stderr, "read error\n");
            exit(EXIT_FAILURE);
        }

        if (close(cfd) == -1) {
            fprintf(stderr, "close error\n");
            exit(EXIT_FAILURE);
        }
    }
}
