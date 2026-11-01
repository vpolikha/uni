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

    sid = semget(KEY, 4, IPC_CREAT | 0666);
    if (-1 == sid){
        perror("semget error!");
        return 1;
    }

    unsigned short values[4] = {1, 0, 5, 2};
    union semun arg;
    arg.array = values;

    res = semctl(sid, 0, SETALL, arg);
    if (-1 == res){
        perror("semctl SETALL error!");
        return 1;
    }

    res = semctl(sid, 0, GETALL, arg);
    if (-1 == res){
        perror("semctl GETVAL error!");
        return 1;
    }
    printf("sid = %i value = %i\n", sid, arg.array[0]);
    printf("sid = %i value = %i\n", sid, arg.array[1]);
    printf("sid = %i value = %i\n", sid, arg.array[2]);
    printf("sid = %i value = %i\n", sid, arg.array[3]);

    struct semid_ds info;
    arg.buf = &info;

    res = semctl(sid, 0, IPC_STAT, arg);
    if (-1 == res){
        perror("semctl IPC_STAT error!");
        return 1;
    }
    printf("sid = %i amt of semaphores = %li\n", sid, info.sem_nsems);

    res = semctl(sid, 0, IPC_RMID);
    if (-1 == res){
        perror("semctl IPC_RMID error!");
        return 1;
    }
}