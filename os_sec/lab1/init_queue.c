#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "message_queue.h"

int init_queue()
{
    int msqid;
    msqid = msgget(MSQ_ID, IPC_CREAT | S_IRUSR | S_IWUSR);
    //msqid = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);

    if (msqid == -1) {
        fprintf(stderr, "Error creating message queue.");
        exit(EXIT_FAILURE);
    }

    return msqid;
}
