#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** args){
    if(argc < 1){
        printf("I need a file parameter\n");
        return -1;
    }

    int fd = open(args[1], O_RDONLY);
    if(fd<0){
        printf("Failed to open for O_RDONLY\n");
        return -1;
    }
    close(fd);
    fd = open(args[1], O_WRONLY);
    if(fd<0){
        printf("Failed to open for O_WRONLY\n");
        return -1;
    }
    close(fd);
    fd = open(args[1], O_RDWR);
    if(fd<0){
        printf("Failed to open for O_RDWR\n");
        return -1;
    }
    close(fd);
    fd = open(args[1], O_SYNC | O_TRUNC);
    if(fd<0){
        printf("Failed to open for  O_SYNC | O_TRUNC\n");
        return -1;
    }

    fd = open(args[1], O_APPEND);
    if(fd<0){
        printf("Failed to open for  O_SYNC | O_TRUNC\n");
        return -1;
    }
    close(fd);
    return 0;
}