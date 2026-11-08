#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unix_datagram.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);       /* Create server socket */
    if (sfd == -1) {
        fprintf(stderr, "socket error\n");
        exit(EXIT_FAILURE);
    }

    /* Construct well-known address and bind server socket to it */

    if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1) {
        fprintf(stderr, "Server socket path too long\n");
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        fprintf(stderr, "remove error\n");
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "bind error\n");
        exit(EXIT_FAILURE);
    }

    /* Receive messages, convert to uppercase, and return to client */

    for (;;) {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

        if (numBytes == -1) {
            fprintf(stderr, "recvfrom error\n");
            exit(EXIT_FAILURE);
        }

        printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

        for (j = 0; j < numBytes; j++) {
            buf[j] = toupper((unsigned char) buf[j]);
        }

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
            fprintf(stderr, "sendto error\n");
            exit(EXIT_FAILURE);
        }
    }
}
