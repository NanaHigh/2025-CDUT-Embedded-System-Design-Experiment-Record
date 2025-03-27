#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 8888
#define LED_BASE_PATH "/sys/devices/platform/leds-gpio/leds/led"
#define LED_BRIGHTNESS "/brightness"

//设置LED灯的状态：value=1 亮；value=0 灭
void write_led_bit(const char *path, int value)
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

// LED控制函数
void write_led(int led_num, int state) {
    switch (led_num) {
        case 1: write_led_bit(LED_BASE_PATH "1" LED_BRIGHTNESS, state); break;
        case 2: write_led_bit(LED_BASE_PATH "2" LED_BRIGHTNESS, state); break;
        case 3: write_led_bit(LED_BASE_PATH "3" LED_BRIGHTNESS, state); break;
        case 4: write_led_bit(LED_BASE_PATH "4" LED_BRIGHTNESS, state); break;
    }
}

// 命令解析函数
void parse_command(unsigned char cmd) {
    int led_num = (cmd >> 4) & 0x0F;  // 高4位
    int action = cmd & 0x0F;           // 低4位

    if (led_num > 3) return;  // 只支持4个LED
    write_led(led_num, action); // action 1:亮 0:灭
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    unsigned char buffer[1];

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("LED Control Server Started\n");

    while(1) {
        // 接受连接
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        printf("New connection accepted\n");

        // 处理命令
        while(1) {
            int valread = read(client_fd, buffer, 1);
            if (valread <= 0) break;
            
            parse_command(buffer[0]);
            printf("Received command: 0x%02X\n", buffer[0]);
        }

        close(client_fd);
        printf("Connection closed\n");
    }

    return 0;
}