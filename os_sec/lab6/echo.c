
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "unix_stream.h"

#define BACKLOG 5

void *client_handler(void *arg)
{
    int cfd = *(int *)arg;
    free(arg);

    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, sizeof(buf))) > 0) {
        ssize_t total = 0;

        while (total < numRead) {
            ssize_t n = write(cfd, buf + total, numRead - total);

            if (n == -1) {
                perror("write");
                close(cfd);
                return NULL;
            }

            total += n;
        }
    }

    if (numRead == -1)
        perror("read");

    close(cfd);
    return NULL;
}

int main(void)
{
    struct sockaddr_un addr;
    int sfd, cfd;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        perror("remove");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH,
            sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *)&addr,
             sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Echo server started.\n");

    for (;;) {
        cfd = accept(sfd, NULL, NULL);

        if (cfd == -1) {
            if (errno == EINTR)
                continue;

            perror("accept");
            continue;
        }

        int *arg = malloc(sizeof(int));

        if (arg == NULL) {
            perror("malloc");
            close(cfd);
            continue;
        }

        *arg = cfd;

        pthread_t tid;
        int err = pthread_create(&tid, NULL,
                                 client_handler, arg);

        if (err != 0) {
            fprintf(stderr, "pthread_create: %s\n",
                    strerror(err));
            free(arg);
            close(cfd);
            continue;
        }

        pthread_detach(tid);
    }

    close(sfd);
    remove(SV_SOCK_PATH);

    return 0;
}
