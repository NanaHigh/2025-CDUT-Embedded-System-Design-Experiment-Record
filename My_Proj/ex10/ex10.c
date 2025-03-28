#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

// LED控制相关头文件
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

void* led_thread(void* arg) {
    uint8_t led_val = 0x01;
    while(1) {
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
    return NULL;
}

void* uart_thread(void* arg) {
    int fd;
    char *uart_dev = "/dev/ttySAC1"; // 根据实际设备节点修改
    char buffer[128];
    
    if ((fd = open(uart_dev, O_RDWR | O_NOCTTY)) < 0) {
        perror("open");
        return -1;
    }

    if (uart_init(fd, 9600, 8, 'N', 1) < 0) {
        fprintf(stderr, "UART init failed\n");
        close(fd);
        return -1;
    }

    // 串口通信逻辑
    while(1) {
        // 发送数据
        char *msg = "Hello World!\n";
        int n = write(fd, msg, strlen(msg));
        if (n < 0) {
            perror("write");
        } else {
            printf("Sent %d bytes\n", n);
        }
        sleep(1);
        
        // 接收数据（可选）
        /*int n = read(fd, buffer, sizeof(buffer));
        if(n > 0) {
            buffer[n] = '\0';
            printf("Received: %s", buffer);
        }*/
        
        sleep(1);
    }
    
    close(fd);
    return NULL;
}

int main() {
    pthread_t led_tid, uart_tid;

    // 创建LED线程
    if (pthread_create(&led_tid, NULL, led_thread, NULL)) {
        perror("LED thread create failed");
        return -1;
    }

    // 创建串口线程
    if (pthread_create(&uart_tid, NULL, uart_thread, NULL)) {
        perror("UART thread create failed");
        return -1;
    }

    printf("LED Thread ID: %lx\n", (unsigned long)led_tid);
    printf("UART Thread ID: %lx\n", (unsigned long)uart_tid);

    // 等待线程结束（实际不会执行到这里）
    pthread_join(led_tid, NULL);
    pthread_join(uart_tid, NULL);

    return 0;
}