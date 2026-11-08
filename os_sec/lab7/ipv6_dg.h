#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define BUF_SIZE 10 /* Maximum size of messages exchanged between client and server */
#define PORT_NUM 50002 /* Server port number */

void errExit(char *err)
{
   fprintf(stderr, "%s error\n", err);
   exit(EXIT_FAILURE);
}
