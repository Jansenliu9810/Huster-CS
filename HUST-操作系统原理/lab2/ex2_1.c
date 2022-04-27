#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <signal.h>

int a;      //定义一个整型变量a作为计算数
int i;      //定义一个整型变量i作为总数
int semid;  //信号灯id

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

// 线程1计算器创建
void subp1(void){
    printf("线程1计算器创建成功！\n");
    while(1){
        P(semid, 0);    //访问write的信号灯
        if(i == 101){   //判定总数是否累计到100次计算
            printf("线程1计算器计算完成！\n");
            V(semid, 1);
            break;
        }
        a += i;
        if(a % 2 == 1){     //当前计算数判断是否为奇数或偶数
            V(semid, 1);    //判断为奇数
        }
        else{
            V(semid, 2);    //判断为偶数
        }
        i++;
    }
    printf("线程1结束并退出！\n");
    pthread_exit(0);
}

// 线程2奇数打印器创建
void subp2(void){
    printf("线程2奇数打印器创建成功！\n");
    while(1){
        P(semid, 1);    //访问奇数信号灯
        if(a == 5050){
            printf("奇数打印完成！\n");
            break;
        }
        printf("奇数打印为： %d\n", a);
        V(semid, 0);
    }
    printf("线程2奇数打印器结束并关闭！\n");
    pthread_exit(0);
}

// 线程3偶数打印器创建
void subp3(void){
    printf("线程3偶数打印器创建成功！\n");
    while(1){
        P(semid, 2);    //访问偶数信号灯
        if(a == 5050){
            printf("偶数打印为： %d\n", a);
            V(semid, 0);
            printf("偶数打印完成！\n");
            break;
        }
        printf("偶数打印为： %d\n", a);
        V(semid, 0);
    }
    printf("线程3偶数打印器结束并关闭！\n");
    pthread_exit(0);
}

int main(){
    semid = semget(IPC_PRIVATE, 3, IPC_CREAT|0666); //系统调用semget()创建一个新的信号灯集
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);  //写置为1
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);  //奇数可读置0
    arg.val = 0;
    semctl(semid, 2, SETVAL, arg);  //偶数可读置0

    a = 0;  //初始化
    i = 1;  //计算值初始为1
    
    pthread_t id1, id2, id3;        //设定子线程1，2，3
    int ret1, ret2, ret3;
    ret1 = pthread_create(&id1, NULL, (void*)subp1, NULL);
    ret2 = pthread_create(&id2, NULL, (void*)subp2, NULL);
    ret3 = pthread_create(&id3, NULL, (void*)subp3, NULL);

    if(ret1 != 0 || ret2 != 0 || ret3 != 0){
        printf("线程创建失败！\n");
        exit(0);
    }
    pthread_join(id1, NULL);    //等待线程1运行结束
    pthread_join(id2, NULL);    //等待线程2运行结束
    pthread_join(id3, NULL);    //等待线程3运行结束

    printf("计算和打印全部完成！\n");
    return 0;
}