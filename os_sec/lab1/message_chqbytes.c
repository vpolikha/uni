#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "message_queue.h"

int main(int argc, char *argv[])
{
    struct msqid_ds ds;
    int msqid;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s msqid max-bytes\n", argv[0]);
    }

    /* Retrieve copy of associated data structure from kernel */
    msqid = atoi(argv[1]);

    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        fprintf(stderr, "msgctl error");
        exit(EXIT_FAILURE);
    }

    ds.msg_qbytes = atoi(argv[2]);
    /* Update associated data structure in kernel */
    if (msgctl(msqid, IPC_SET, &ds) == -1) {
        fprintf(stderr, "msgctl error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
