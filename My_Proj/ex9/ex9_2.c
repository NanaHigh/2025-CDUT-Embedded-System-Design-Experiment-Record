#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include "uart_init.h"
#include <stdlib.h>
#include <stdint.h>

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
    int fd;
    char cmd[32];
    static int running = 0;

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

    uint8_t led_val = 0x01; // 初始值为0x01，即LED1亮
    while (1) {
        if (read(fd, cmd, 32) > 0) {
            if (strstr(cmd, "start")) running = 1;
            if (strstr(cmd, "stop")) running = 0;
        }
            
        if (running) {
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
        } else {
            // write_led_bits(0);
        }
    }

    close(fd);
    return 0;
}