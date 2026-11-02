#include <sys/sem.h>
#include <stdio.h>

#define KEY 1

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(){
    int sid, res;

    sid = semget(KEY, 1, IPC_CREAT | 0666);
    if (-1 == sid){
        perror("semget error!");
        return 1;
    }
    
    struct sembuf buf = {0, 1, 0};

    res = semop(sid, &buf, 1);
    if (-1 == res){
        perror("semop error");
        return 1;
    }

    //union semun arg;
    //arg.val = 1;

    //res = semctl(sid, 0, SETVAL, arg);
    //if (-1 == res){
    //    perror("semctl error!");
    //    return 1;
    //}

    res = semctl(sid, 0, GETVAL);
    if (-1 == res){
        perror("semctl GETVAL error!");
        return 1;
    }
    printf("sid = %i value = %i", sid, res);

    //res = semctl(sid, 0, IPC_RMID);
    //if (-1 == res){
    //    perror("semctl IPC_RMID error!");
    //    return 1;
    //}
}
