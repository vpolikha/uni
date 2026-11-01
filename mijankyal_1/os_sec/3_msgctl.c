#include <sys/msg.h>
#include <stdio.h>

#define KEY 1
#define TYPE 1

int main(){
    struct msqid_ds info;
    int res, mid;

    mid = msgget(KEY, 0666);
    if (-1 == mid){
        perror("msgget error!");
        return 1;
    }

    res = msgctl(mid, IPC_STAT, &info);
    if (-1 == res){
        perror("msgctl IPC_STAT error!");
        return 1;
    }
    printf("mid %i capacity = %lu\n", mid, info.msg_qbytes);
    
    info.msg_qbytes = 200;
    res = msgctl(mid, IPC_SET, &info);
    if (-1 == res){
        perror("msgctl IPC_SET error!");
        return 1;
    }
    printf("mid %i capacity = %lu\n", mid, info.msg_qbytes);

    res = msgctl(mid, IPC_RMID, NULL);
    if (-1 == res){
        perror("msgctl IPC_RMID error!");
        return 1;
    }
    return 0;
}