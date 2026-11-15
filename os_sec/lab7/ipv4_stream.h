#ifndef IPV4_STREAM_H
#define IPV4_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 5
#define MAX_CLIENTS 10
#define BUF_SIZE 1024
#define PORT_NUM 50002

#endif

