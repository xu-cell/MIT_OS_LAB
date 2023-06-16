#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 当我第一个线程是筛

//


void re_pipe(int pipe_id) {
    int p[2]; 
    pipe(p);
    // 读取左边的线程管道的第一个质数
    int first_value;
    int n = read(pipe_id,&first_value,sizeof(int));
    if(n > 0) {printf("prime %d\n",first_value);}
    else exit(0);
    
    int value;   
    while(read(pipe_id,&value,sizeof(int))) {
        if(value % first_value != 0) {
            write(p[1],&value,sizeof(int));
        }
    }
    close(p[1]);
     //printf("-----log----\n");
    int pid = fork();
    if(pid > 0) {    
       wait(&pid);
               
    } else {
        //printf("创建一个线程tid = %d\n",getpid());
        re_pipe(p[0]);
    }
    exit(0);
}
int main(int argc, char *argv[]) {
    int p1[2];
    pipe(p1);
    for(int i = 2;i <= 35;i++) {
        write(p1[1],&i,sizeof(int));
    } 
    close(p1[1]);
    int pid = fork();
    if(pid > 0) {
     
    wait(&pid);
    } else {
        re_pipe(p1[0]); // 传入左边线程的读端
    }
    exit(0);
}