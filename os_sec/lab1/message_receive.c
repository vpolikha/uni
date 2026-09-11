#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "message_queue.h"

int main()
{
    int msqid = init_queue();
    // int msqid = 4;
    int type = 20;
    size_t maxBytes = MAX_MTEXT;
    ssize_t msgLen;
    struct mbuf msg; /* Message buffer for msgrcv() */
    msgLen = msgrcv(msqid, &msg, maxBytes, type, IPC_NOWAIT | MSG_NOERROR);

    if (msgLen == -1) {
        fprintf(stderr, "msgrcv");
        exit(-1);
    }

    printf("Received: type=%ld; length=%ld", msg.mtype, (long) msgLen);

    if (msgLen > 0) {
        printf("; body=%s", msg.mtext);
    }

    printf("\n");
    exit(EXIT_SUCCESS);
}
