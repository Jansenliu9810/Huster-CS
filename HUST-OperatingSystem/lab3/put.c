#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stddef.h>
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
char *buf[3];                   //设立缓冲区个数

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
    // 获取key的3个共享存储区
    shmid1 = shmget(SHMKEY1, 100*sizeof(char), 0666);
    shmid2 = shmget(SHMKEY2, 100*sizeof(char), 0666);
    shmid3 = shmget(SHMKEY3, 100*sizeof(char), 0666);

    // 建立数组形式环形缓冲区
    buf[0] = (char*)shmat(shmid1, 0, 0);    /*获取shmid1的首地址*/
    buf[1] = (char*)shmat(shmid2, 0, 0);    /*获取shmid2的首地址*/
    buf[2] = (char*)shmat(shmid3, 0, 0);    /*获取shmid3的首地址*/

    semid = semget((key_t)13, 3, IPC_CREAT|0666);   // 获取Key的信号灯

    FILE *fileprocess;
    if((fileprocess = fopen("output.txt", "w")) == NULL){   /*创建一个名为output.txt的目标写入文件*/
        printf("文件打开出错!\n");
        exit(0);
    }

    int i = 0;
    int j = 0;
    char c[100] = "EOF";
    while(1){
        P(semid, 1);
        if(strcmp(buf[i], c) == 0){
            fclose(fileprocess);
            break;
        }
        V(semid, 0);            //信号灯V操作
        fputs(buf[i], fileprocess);     /*从共享缓冲区内提取数据并写入文件*/
        printf("PUT 占用共享存储区 %d 的时间 %d\n", j++, i);
        i = (i + 1) % 3;        //移动环形缓冲区指针
    }
    V(semid, 2);
    printf("PUT 操作完成！\n");
    exit(0);
}