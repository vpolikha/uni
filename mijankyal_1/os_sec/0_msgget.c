#include <sys/msg.h>
#include <stdio.h>

#define KEY 1 

int main(){
    int mid;

    mid = msgget(KEY, IPC_CREAT | 0666);
    if (-1 == mid){
        perror("msgget error!");
        return 1;
    }
    printf("mid = %i\n", mid);
    return 0;
}