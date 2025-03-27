#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8888

void show_help() {
    printf("Usage:\n");
    printf("  led <0-3> <on/off>\n");
    printf("  exit\n");
    printf("Example:\n");
    printf("  led 1 on\n");
    printf("  led 3 off\n");
}

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char input[20];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 输入服务器IP
    char ip[16];
    printf("Enter board IP: ");
    scanf("%15s", ip);
    
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    printf("Connected to %s\n", ip);
    show_help();

    while(1) {
        printf("> ");
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) break;

        if (strcmp(input, "led") == 0) {
            int led_num;
            char action[5];
            scanf("%d %s", &led_num, action);

            if (led_num < 0 || led_num > 3) {
                printf("Invalid LED number (0-3)\n");
                continue;
            }

            unsigned char cmd = 0;
            cmd |= (led_num & 0x0F) << 4;  // 设置高4位
            cmd |= (strcmp(action, "on") == 0) ? 0x00 : 0x01;  // 设置低4位

            send(sock, &cmd, 1, 0);
            printf("Sent command: 0x%02X\n", cmd);
        } else {
            show_help();
        }
    }

    close(sock);
    return 0;
}