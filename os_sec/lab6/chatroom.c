
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "unix_stream.h"

#define BACKLOG 5
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int add_client(int cfd)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == -1) {
            clients[i] = cfd;
            pthread_mutex_unlock(&clients_mutex);
            return 0;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
    return -1;
}

void remove_client(int cfd)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == cfd) {
            clients[i] = -1;
            close(cfd);
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void broadcast(int sender, char *buf, ssize_t numRead)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        int cfd = clients[i];

        if (cfd == -1 || cfd == sender)
            continue;

        ssize_t total = 0;

        while (total < numRead) {
            ssize_t n = write(cfd, buf + total, numRead - total);

            if (n <= 0)
                break;

            total += n;
        }

        if (total < numRead) {
            clients[i] = -1;
            close(cfd);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg)
{
    int cfd = *(int *)arg;
    free(arg);

    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, sizeof(buf))) > 0) {
        broadcast(cfd, buf, numRead);
    }

    if (numRead == -1)
        perror("read");

    printf("Client disconnected: fd=%d\n", cfd);

    remove_client(cfd);

    return NULL;
}

int main(void)
{
    struct sockaddr_un addr;
    int sfd, cfd;

    signal(SIGPIPE, SIG_IGN);

    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i] = -1;

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
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started.\n");

    for (;;) {
        cfd = accept(sfd, NULL, NULL);

        if (cfd == -1) {
            if (errno == EINTR)
                continue;

            perror("accept");
            continue;
        }

        if (add_client(cfd) == -1) {
            fprintf(stderr, "Maximum number of clients reached.\n");
            close(cfd);
            continue;
        }

        int *arg = malloc(sizeof(int));

        if (arg == NULL) {
            perror("malloc");
            remove_client(cfd);
            continue;
        }

        *arg = cfd;

        pthread_t tid;
        int err = pthread_create(&tid, NULL, client_handler, arg);

        if (err != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(err));
            free(arg);
            remove_client(cfd);
            continue;
        }

        pthread_detach(tid);

        printf("Client connected: fd=%d\n", cfd);
    }
}
