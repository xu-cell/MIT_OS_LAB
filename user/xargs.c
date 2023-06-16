#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// #define MAXARG       32  // max exec arguments
#define MYMAXARG 32
void readline(){

}
void runcommd(){};


int main(int argc, char *argv[]) {
    int i;
    int j = 0;
    if(argc < 2) {
        fprintf(2, "Usage: xargs command...\n");
        exit(1);
    }
    char* my_argv[MYMAXARG];
    for(i = 1; i < argc;i++) {
        my_argv[j++] = argv[i];
    }
    // for(int i = 0; i < j;i++) {
    //     //printf("命令所携带的基本参数为：%s\n",my_argv[i]);
    // }

    char buf[1024];
    char *p,*q;
    int n,m;
    m =  0;
    while((n = read(0,buf + m,sizeof(buf)-m-1)) > 0) {
        //printf("读取：%d 字节\n",n);
        m += n; //已经读取的字节数
        buf[m] = '\0';
        p = buf;
        while((q = strchr(p,'\n')) != 0) {
           if(j < MYMAXARG) {
            my_argv[j] = malloc(q-p);
            memcpy(my_argv[j],p,q-p);
            //printf("附带参数为：%s\n",my_argv[j]);
            j++;
           }
           p = q + 1;
        }
        if(m > 0) {
            m -= p - buf;
            memmove(buf,p,m);
        }

    }

    // printf("---------------\n");
    // for(int i = 0; i < MYMAXARG;i++) {
    //      printf("第 %d 个参数：%s\n",i,my_argv[i]);

    // }


    int tid = fork();
    if(tid == 0) {
       
         exec(argv[1],my_argv);
   
    }
    else {
        wait(&tid);
    }
    exit(0);
}