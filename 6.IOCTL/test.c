#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h> //user space IOCTL

#define MAGIC_CHAR 'T'
#define SAY_HELLO _IO(MAGIC_CHAR, 1)
#define CLEAR_BUFFER _IO(MAGIC_CHAR, 2)
#define TEXT_TO_USER _IOR(MAGIC_CHAR, 3, char[500])
#define TEXT_TO_KERNEL _IOW(MAGIC_CHAR, 4, int)

int main(int argc, char** args){
    if(argc < 2){
        printf("I need a file parameter\n");
        return -1;
    }

    int fd = open(args[1], O_RDWR);
    if(fd<0){
        printf("Failed to open for O_RDWR\n");
        return -1;
    }

    ioctl(fd, SAY_HELLO);

    ioctl(fd, CLEAR_BUFFER);

    int hiddenNum;
    if(ioctl(fd, TEXT_TO_USER, &hiddenNum)==0)
        printf("Got a num from kernel space:%#X\n", hiddenNum);
    else
        printf("Somewthing went wrong\n");
    hiddenNum = 0xc0ffee;
    ioctl(fd, TEXT_TO_KERNEL, &hiddenNum);

    close(fd);

    return 0;
}