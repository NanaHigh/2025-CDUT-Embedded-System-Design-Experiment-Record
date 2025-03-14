#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* file_name = "text1.txt";
    char wr_buf[] = "Hello write function!";
    char rd_buf[1000];
    int fd = open(file_name, O_CREAT | O_RDWR, 0777);
    write(fd, wr_buf, strlen(wr_buf));
    read(fd, rd_buf, strlen(wr_buf));
    close(fd);

    fd = open(file_name, O_RDWR, 0777);
    int ret = read(fd, rd_buf, 1000);
    if (ret == -1) {
        return -1;
    }
    printf("File Name: %s\n", file_name);
    printf("File Content: %s\n", rd_buf);
    printf("Length of file: %d\n", ret);

    close(fd);
    return 0;
}