#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 8888
#define BUFSIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    int client_id;
    int sockfd;
} client_info_t;

// 全局变量用于生成客户端ID
static int client_count = 0;
static pthread_mutex_t client_count_mutex = PTHREAD_MUTEX_INITIALIZER;

void* client_handler(void* arg) {
    client_info_t* info = (client_info_t*)arg;
    int client_id = info->client_id;
    int sockfd = info->sockfd;
    free(info);  // 立即释放结构体内存
    
    char buffer[BUFSIZE];
    printf("Client %d connected (socket: %d)\n", client_id, sockfd);

    while(1) {
        memset(buffer, 0, BUFSIZE);
        ssize_t len = recv(sockfd, buffer, BUFSIZE, 0);

        if(len > 0) {
            printf("Client %d: %s", client_id, buffer);
        } else if(len == 0) {
            printf("Client %d disconnected\n", client_id);
            break;
        } else {
            if(errno == EWOULDBLOCK || errno == EAGAIN) continue;
            perror("recv error");
            break;
        }
    }

    close(sockfd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    while(1) {
        client_info_t* info = malloc(sizeof(client_info_t));
        if(!info) {
            perror("malloc failed");
            continue;
        }

        info->sockfd = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addr_len);
        if(info->sockfd < 0) {
            perror("accept error");
            free(info);
            continue;
        }

        // 生成客户端ID（线程安全）
        pthread_mutex_lock(&client_count_mutex);
        info->client_id = ++client_count;
        pthread_mutex_unlock(&client_count_mutex);

        pthread_t tid;
        if(pthread_create(&tid, NULL, client_handler, info)) {
            fprintf(stderr, "Failed to create thread for client %d\n", info->client_id);
            close(info->sockfd);
            free(info);
        } else {
            pthread_detach(tid);
            printf("New client %d accepted (socket: %d)\n", info->client_id, info->sockfd);
        }
    }

    close(server_fd);
    return 0;
}