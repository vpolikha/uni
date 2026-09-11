#include <stdio.h>
#include <stdlib.h>
#include "message_queue.h"

int main()
{
    int msqid = init_queue();

    printf("%d\n", msqid);
    exit(EXIT_SUCCESS);
}
