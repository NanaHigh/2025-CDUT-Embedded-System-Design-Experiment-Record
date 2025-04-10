#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 128

int main() {
    int data_pipe[2];   // 数据传输管道：父写 -> 子读
    int sync_pipe[2];   // 同步管道：子写 -> 父读
    char buf[BUFSIZE];
    pid_t pid;

    // 创建两个管道
    if (pipe(data_pipe) || pipe(sync_pipe)) {
        perror("pipe");
        return -1;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid > 0) { // 父进程
        close(data_pipe[0]);  // 关闭数据管道读端
        close(sync_pipe[1]);  // 关闭同步管道写端

        while (1) {
            printf("Input > ");
            fflush(stdout);  // 强制刷新输出缓冲区

            // 获取用户输入
            if (fgets(buf, BUFSIZE, stdin) == NULL) {
                break;  // 处理 EOF（如 Ctrl+D）
            }

            // 退出命令检测
            if (strcmp(buf, "exit\n") == 0) {
                break;
            }

            // 写入数据到子进程
            write(data_pipe[1], buf, strlen(buf) + 1);

            // 等待子进程同步信号
            char sync_byte;
            if (read(sync_pipe[0], &sync_byte, 1) <= 0) {
                break;  // 子进程异常退出
            }
        }

        // 清理资源
        close(data_pipe[1]);
        close(sync_pipe[0]);
        wait(NULL);  // 等待子进程退出
        exit(0);

    } else { // 子进程
        close(data_pipe[1]);  // 关闭数据管道写端
        close(sync_pipe[0]);  // 关闭同步管道读端

        while (1) {
            ssize_t n = read(data_pipe[0], buf, BUFSIZE);
            if (n > 0) {
                printf("Received: %s", buf);
                fflush(stdout);  // 确保立即输出

                // 发送同步信号给父进程
                write(sync_pipe[1], "A", 1);  // 发送任意1字节
            } else if (n == 0) {
                break;  // 父进程已关闭写端
            } else {
                perror("read");
                break;
            }
        }

        // 清理资源
        close(data_pipe[0]);
        close(sync_pipe[1]);
        exit(0);
    }

    return 0;
}