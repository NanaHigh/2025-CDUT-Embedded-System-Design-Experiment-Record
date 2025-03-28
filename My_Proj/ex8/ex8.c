#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include "uart_init.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/input.h>

/// 设置LED灯的状态：value=1 亮；value=0 灭
void write_led_bit(const char *path, uint8_t value)
{
    int fd;

    fd = open(path, O_RDWR);
    if(fd >= 0)
    {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        write(fd, buffer, bytes);
        close(fd);
    }
    else
    {
        printf("Can not write led : %s\r\n",path);
    }
}

/// 设置LED灯组的状态
void write_led_bits(uint8_t value) {    
    write_led_bit("/sys/devices/platform/leds-gpio/leds/led1/brightness", value & 0x01);
    write_led_bit("/sys/devices/platform/leds-gpio/leds/led2/brightness", (value >> 1) & 0x01);
    write_led_bit("/sys/devices/platform/leds-gpio/leds/led3/brightness", (value >> 2) & 0x01);
    write_led_bit("/sys/devices/platform/leds-gpio/leds/led4/brightness", (value >> 3) & 0x01);
}

int main() {
    int i = 1;
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        printf("fork failed!\n");
    } 
    // 父进程中 fork 的返回值为子进程 id
    else if (pid > 0) {
        i++;
        printf("\nThe parent i = %d\n", i);
        printf("In parent return value of fork is %d\n", pid);
        printf("The parent pid is %d\n", getpid());
        printf("The parent ppid is %d\n", getppid());

        int fd;
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

        while (1) { // 让父进程持续运行
            int n = write(fd, msg, strlen(msg));
            if (n < 0) {
                perror("write");
            } else {
                printf("Sent %d bytes\n", n);
            }
            sleep(1);
        }

        close(fd);
    } 
    // 子进程中 fork 的返回值为 0
    else if (pid == 0) {
        i++;
        printf("\nThe child i = %d\n", i);
        printf("In child return value of fork is %d\n", pid);
        printf("The child pid is %d\n", getpid());
        printf("The child ppid is %d\n", getppid());

        uint8_t led_val = 0x01; // 初始值为0x01，即LED1亮
        while (1){ // 让子进程持续运行
            write_led_bits(~led_val);
            usleep(500000); // 延时500ms
            led_val <<= 1; // 左移一位
            if (led_val > 0x08) {
                while (led_val != 0x01) {
                    led_val >>= 1; // 右移一位
                    write_led_bits(~led_val);
                    usleep(500000); // 延时500ms
                }
            }
        }
    }

    return 0;
}
