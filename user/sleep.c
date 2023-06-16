#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int timeinterrupts;
    if(argc <= 1) {
        fprintf(2,"usage: please type slepp number\n");
        exit(1);
    }
    // 获取外部输入的睡眠时间
    timeinterrupts = atoi(argv[1]);
    sleep(timeinterrupts);
    exit(0);
}