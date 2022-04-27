#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>


int semid;      //信号灯集ID
int selled;     //已售票数
int total;      //总数

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

// 创建售票线程1
void subp1(void){
    printf("线程1创建成功！\n");
    int sell1 = 1;      //线程1售票票数统计
    while(1){
        P(semid, 0);    //访问信号灯P
        if(selled == total){    //判断机票已售空
            printf("所有机票已售空！\n");
            V(semid, 0);
            break;
        }
        printf("线程1出售机票： %d 张\n", selled);
        sell1++;
        selled++;
        V(semid, 0);    //V操作唤醒下一个
        sleep(1);
    }
    printf("线程1已售机票： %d 张\n", sell1 - 1);
    printf("售票线程1退出\n");
    pthread_exit(0);
}

// 创建售票线程2
void subp2(void){
    printf("线程2创建成功！\n");
    int sell2 = 1;      //线程2售票票数统计
    while(1){
        P(semid, 0);    //访问信号灯P
        if(selled == total){    //判断机票已售空
            printf("所有机票已售空！\n");
            V(semid, 0);
            break;
        }
        printf("线程2出售机票： %d 张\n", selled);
        sell2++;
        selled++;
        V(semid, 0);    //V操作唤醒下一个
        sleep(1);
    }
    printf("线程2已售机票： %d 张\n", sell2 - 1);
    printf("售票线程2退出！\n");
    pthread_exit(0);
}

int main(void){
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666); //系统调用semget()创建一个新的信号灯集
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    selled = 0;
    total = 10;
    pthread_t id1;  //设定子线程id1
    pthread_t id2;  //设定子线程id2

    int ret1, ret2;
    ret1 = pthread_create(&id1, NULL, (void*)subp1, NULL);
    ret2 = pthread_create(&id2, NULL, (void*)subp2, NULL);

    if(ret1 != 0 || ret2 != 0){
        printf("线程创建失败！\n");
        exit(0);
    }
    pthread_join(id1, NULL);    //等待线程1运行结束
    pthread_join(id2, NULL);    //等待线程2运行结束

    printf("总共销售了 %d 张机票\n", selled);
    return 0;
}