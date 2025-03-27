#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "uart_init.h"

int main() {
    int fd;
    char buf[512];
    char *uart_dev = "/dev/ttySAC1"; // 根据实际设备节点修改

    if ((fd = open(uart_dev, O_RDWR | O_NOCTTY)) < 0) {
        perror("open");
        return -1;
    }

    if (uart_init(fd, 9600, 8, 'N', 1) < 0) {
        fprintf(stderr, "UART init failed\n");
        close(fd);
        return -1;
    }

    write(fd, "Please input\r\n", 13); // 发送提示信息

    char tmp[50] = {0};

    while (1) {
        int n = read(fd, buf, sizeof(buf)-1);
        if (n > 0) {
            sprintf(tmp, "Received: %s", buf);
            write(fd, tmp, strlen((const char *)tmp)); // 回显数据
            buf[n] = '\0';
        }
    }

    close(fd);
    return 0;
}