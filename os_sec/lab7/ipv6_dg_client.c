#include <string.h>
#include "ipv6_dg.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 3) {
        errExit("Too few arguments.\n");
    }

    /* Create a datagram socket; send to an address in the IPv6 domain */

    sfd = socket(AF_INET6, SOCK_DGRAM, 0);      /* Create client socket */

    if (sfd == -1) {
        errExit("socket");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(PORT_NUM);

    if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0) {
        errExit("inet_pton failed");
    }

    /* Send messages to server; echo responses on stdout */

    for (j = 2; j < argc; j++) {
        msgLen = strlen(argv[j]);

        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msgLen) {
            errExit("sendto");
        }

        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);

        if (numBytes == -1) {
            errExit("recvfrom");
        }

        printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}
