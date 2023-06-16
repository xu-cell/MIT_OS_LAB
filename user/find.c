#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* 
fmtname(char* path) {
  char* p = path + strlen(path) - 1; // 指向最后一个字符
  while (p >= path && *p != '/') {
    p--;  // 逆向查找斜杠
  }
  return p + 1; // 返回斜杠后的字符作为文件名
}

void find(char *path, char *name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type) {
    case T_FILE: 
        if(strcmp(name,fmtname(path)) == 0) {
            printf("%s/%s\n",path,name);
        }
        break;

    case T_DIR:

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf,path);
    p = buf + strlen(buf);
    *p++ = '/'; // ./
    while(read(fd,&de,sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
         memmove(p, de.name, DIRSIZ); // ./name
         p[DIRSIZ] = 0; 
         if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
        }  

        
        switch(st.type) {
            case T_FILE:
            if(strcmp(name,fmtname(buf)) == 0) {
            printf("%s/%s\n",path,name);
            break;
            }
            case T_DIR:

            if((strcmp(fmtname(buf),".") != 0) && (strcmp(fmtname(buf),"..") != 0)) {

                find(buf,name);
            }
            break;

        }
    }
    break;

  }
  close(fd);
}
int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("please type find path filename\n");
        exit(0);
    }
    find(argv[1],argv[2]);
    exit(0);    
}