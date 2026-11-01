#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define KEY 1
#define TYPE 1

typedef struct msg{
    long type;
    char text[100];
} msg;

int main(){
    int msid, snd;

    msid = msgget(KEY, IPC_CREAT | 0666);
    if (-1 == msid){
        perror("msgget error!");
        return 1;
    }

    printf("operating on  mid = %i\n", msid);

    msg const greeting = {TYPE, "Hello there!"};
    snd = msgsnd(msid, &greeting, strlen(greeting.text) + 1, IPC_NOWAIT);

    if (-1 == snd){
        perror("msgsnd error!");
        return 1;
    }
    printf("message sent successfully!\n");

    return 0;
}