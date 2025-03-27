#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "uart_init.h"

int main() {
    int fd, i = 10;
    char *uart_dev = "/dev/ttySAC1"; // 根据实际设备节点修改
    char *msg = "Hello World!\n";

    if ((fd = open(uart_dev, O_RDWR | O_NOCTTY)) < 0) {
        perror("open");
        return -1;
    }

    if (uart_init(fd, 9600, 8, 'N', 1) < 0) {
        fprintf(stderr, "UART init failed\n");
        close(fd);
        return -1;
    }

    while (i--) {
        int n = write(fd, msg, strlen(msg));
        if (n < 0) {
            perror("write");
        } else {
            printf("Sent %d bytes\n", n);
        }
        sleep(1);
    }

    close(fd);
    return 0;
}