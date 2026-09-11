#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include "message_queue.h"

int main()
{
    int msqid = init_queue();
    // int msqid = 4;
    struct mbuf msg; /* Message buffer for msgsnd() */
    char *str;
    str = "Test message";
    int msgLen = strlen(str);
    msg.mtype = 20;
    memcpy(msg.mtext, str, msgLen);

    if (msgsnd(msqid, &msg, msgLen, 0) == -1) {
        fprintf(stderr, "%d\n", errno);
        exit(EXIT_FAILURE);
    }
}
