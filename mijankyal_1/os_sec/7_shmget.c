#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define KEY 1

int main(){
    int sid, res;
    void *p;

    sid = shmget(KEY, 4096, IPC_CREAT | 0666);
    if (-1 == sid){
        perror("shmget error!");
        return 1;
    }

    p = shmat(sid, NULL, 0);
    if ((void *)-1 == p){
        perror("shmat error!");
        return 1;
    }

    strcpy((char *)p, "HI THERE!");
    
    struct shmid_ds buf;
    res = shmctl(sid, IPC_STAT, &buf);
    if (-1 == res){
        perror("shmctl error!");
        return 1;
    }
    printf("%li", buf.shm_segsz);
    
    shmdt(p);
    return 0;
}
