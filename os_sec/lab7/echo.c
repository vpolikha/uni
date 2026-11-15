#include "ipv4_stream.h"

void *client_handler(void *arg)
{
    int cfd = *(int *)arg;
    free(arg);

    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, sizeof(buf))) > 0) {
        ssize_t total = 0;

        while (total < numRead) {
            ssize_t n = write(cfd, buf + total,
                              numRead - total);

            if (n == -1) {
                if (errno == EINTR)
                    continue;

                perror("write");
                close(cfd);
                return NULL;
            }

            if (n == 0)
                break;

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
    struct sockaddr_in addr;
    int sfd, cfd;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT_NUM);

    if (bind(sfd, (struct sockaddr *)&addr,
             sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("IPv4 Echo server started on port %d.\n",
           PORT_NUM);

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
    return 0;
}

