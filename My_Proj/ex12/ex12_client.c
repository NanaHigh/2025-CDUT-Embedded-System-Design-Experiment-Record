#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFSIZE 1024

void* send_thread(void* arg) {
    int sockfd = *(int*)arg;
    char buffer[BUFSIZE];
    
    while(1) {
        memset(buffer, 0, BUFSIZE);
        if(fgets(buffer, BUFSIZE, stdin) == NULL) break;
        ssize_t sent = send(sockfd, buffer, strlen(buffer), 0);
        if(sent <= 0) {
            perror("send error");
            break;
        }
    }
    return NULL;
}

void* recv_thread(void* arg) {
    int sockfd = *(int*)arg;
    char buffer[BUFSIZE];
    
    while(1) {
        memset(buffer, 0, BUFSIZE);
        int len = recv(sockfd, buffer, BUFSIZE-1, 0);
        if(len > 0) {
            buffer[len] = '\0';
            printf("Server: %s", buffer);
        } else if(len == 0) {
            printf("Server closed connection\n");
            break;
        } else {
            perror("recv error");
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    pthread_t tid_send, tid_recv;

    if(argc != 2) {
        printf("Usage: %s <server IP>\n", argv[0]);
        return -1;
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return -1;
    }

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        return -1;
    }

    printf("Connected to server!\n");

    pthread_create(&tid_send, NULL, send_thread, &sock);
    pthread_create(&tid_recv, NULL, recv_thread, &sock);

    pthread_join(tid_send, NULL);
    pthread_join(tid_recv, NULL);

    close(sock);
    return 0;
}