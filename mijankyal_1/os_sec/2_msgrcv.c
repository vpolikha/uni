#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>

#define KEY 1
#define TYPE 1

typedef struct msg{
    long type;
    char text[100];
} msg;

int main(){
    int mid;
    msg message;
    ssize_t size;

    mid = msgget(KEY, 0666);
    if (-1 == mid){
        perror("msgget error!");
        return 1;
    }

    size = msgrcv(mid, &message, sizeof(message.text), TYPE, 0);
    if (-1 == size){
        perror("msgrcv error!");
        return 1;
    }
    write(1, message.text, size);

    return 0;
}