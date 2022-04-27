#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#define SIZE 100


//创建两个管道
int pipefd[2];
//创建两个进程号
static int pid1, pid2;

//设置父进程和两个子进程的进程推出机制
void handle1(int sig_no);
void handle2(int sig_no);
void mainhandler(int sig_no);

//main
int main(int argc, char *argv[]){
    signal(SIGINT, mainhandler);    //创建父进程的软中断信号
    pipe(pipefd);

    int sendcounter = 1;
    
    //创建子进程1和2
    pid1 = fork();
    pid2 = fork();
    
    if(sendcounter == 1){
        if(pid1 == 0){
            printf("子进程1创建成功！\n");
            //设置子进程1 信号
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, handle1);
        }
        else if(pid2 == 0){
            printf("子进程2创建成功！\n");
            //设置子进程2 信号
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR2, handle2);
        }
        else{
            close(pipefd[0]);
            char buffer[SIZE];
            while(1){
                printf(buffer, "\nI send you %u times", sendcounter);
                write(pipefd[1], buffer, sizeof(buffer));
                sleep(1);
                sendcounter++;
            }
        }
    }
    else{
        close(pipefd[0]);
        char buffer[SIZE];
        while(1){
            sprintf(buffer, "\nI send you %u times", sendcounter);
            write(pipefd[1], buffer, sizeof(buffer));
            sleep(1);
            sendcounter++;
        }

        if(pid1 == 0){
            printf("子进程1创建成功！\n");
            //设置子进程1 信号
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR1, handle1);

            char buffer[SIZE];
            ssize_t size;
            int sendcounter1 = 1;   //设置一个计数器1
            while(1){
                close(pipefd[1]);
                memset(buffer, 0, SIZE);
                size = read(pipefd[0], buffer, sizeof(buffer));
                printf("1 Received data %s \n", buffer);
                sendcounter1++;
            }
        }
        else if(pid2 == 0){
            //设置子进程2 信号
            printf("子进程2创建成功！\n");
            signal(SIGINT, SIG_IGN);
            signal(SIGUSR2, handle2);

            close(pipefd[1]);
            char buffer[SIZE];
            ssize_t size;
            int sendcounter2 = 1;  //设置一个计数器2
            while(1){
                memset(buffer, 0, SIZE);
                size = read(pipefd[0], buffer, sizeof(buffer));
                printf("2 Received data %s \n", buffer);
                sendcounter2++;
            }
        }
    }
    
    //父进程
    //int num = 10;
    
    // num += 20;
    // printf('父进程： %d', num);

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    close(pipefd[1]);
    close(pipefd[0]);

    return 0;
}


//handle1 终止子进程1
void handle1(int sig_no){
    close(pipefd[0]);
    close(pipefd[1]);
    printf("\nChild Process 1 is killed by Parent!\n");
    exit(0);
}

//handle2 终止子进程2
void handle2(int sig_no){
    close(pipefd[0]);
    close(pipefd[1]);
    printf("\nChild Process 2 is Killed by Parent!\n");
    exit(0);
}

//mainhandle Kill终止两个子进程
void mainhandler(int sig_no){
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR2);
    printf("\nParent Process is Killed!\n");
    exit(0);
}

