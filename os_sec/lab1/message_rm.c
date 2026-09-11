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
    int j;
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [msqid...]\n", argv[0]);
    }

    for (j = 1; j < argc; j++) {
        if (msgctl(atoi(argv[j]), IPC_RMID, NULL) == -1) {
            fprintf(stderr, "msgctl %s", argv[j]);
        }
    }

    exit(EXIT_SUCCESS);
}
