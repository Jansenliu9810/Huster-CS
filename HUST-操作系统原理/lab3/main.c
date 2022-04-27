#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>

#define SHMKEY1 1111
#define SHMKEY2 2222
#define SHMKEY3 3333


int semid;                      //定义一个信号灯id
int shmid1, shmid2, shmid3;     //定义3个共享存储区
char *addr1, *addr2, *addr3;     //定义3个共享缓冲区地址
int pid_p, pid_g;


union semun{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

// P操作接口
void P(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;    //信号灯值-1（s值-1）
    sem.sem_flg = 0;    //操作标记：0或IPC_NOWAIT等
    semop(semid, &sem, 1); //1：表示执行命令的个数
    return;
}

// V操作接口
void V(int semid, int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;     //信号灯值+1
    sem.sem_flg = 0;    //操作标记：0
    semop(semid, &sem, 1);
    return;
}

int main(){
    // 创建3个共享存储区
    shmid1 = shmget(SHMKEY1, 100*sizeof(char), 0666|IPC_CREAT);
    shmid2 = shmget(SHMKEY2, 100*sizeof(char), 0666|IPC_CREAT);
    shmid3 = shmget(SHMKEY3, 100*sizeof(char), 0666|IPC_CREAT);

    semid = semget((key_t)13, 3, IPC_CREAT|0666);   //key信号灯创建，key_t是共享存储区名字
    union semun arg1, arg2, arg3;                   
    arg1.val = 3;
    semctl(semid, 0, SETVAL, arg1);     /*对信号灯的控制操作*/
    arg2.val = 0;
    semctl(semid, 1, SETVAL, arg2);
    arg3.val = 0;
    semctl(semid, 2, SETVAL, arg3);

    // 进程控制，系统调用GET和PUT
    if((pid_g = fork()) == 0){
        puts("GET 创建成功！\n");
        execv("./get", NULL);
    }
    else if((pid_p = fork()) == 0){
        puts("PUT 创建成功！\n");
        execv("./put", NULL);
    }

    wait(1);
    wait(1);

    printf("删除共享存储区并释放资源\n");
    shmctl(shmid1, IPC_RMID, 0);    /*撤销共享存储区，释放资源*/
    shmctl(shmid2, IPC_RMID, 0);
    shmctl(shmid3, IPC_RMID, 0);

    printf("删除信号灯id并释放资源\n");
    semctl(semid, IPC_RMID, 0);     /*删除信号灯id，释放资源*/

    return 0;
}