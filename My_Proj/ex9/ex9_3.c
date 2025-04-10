#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 128
#define FILENAME "abc.txt"

volatile sig_atomic_t flag = 0;
FILE *fp = NULL;

// 信号处理函数
void sig_handler(int signo) {
    if (signo == SIGINT) {
        flag = 1;
    }
}

int main() {
    int data_pipe[2];   // 数据传输管道：父写 -> 子读
    int sync_pipe[2];   // 同步管道：子写 -> 父读
    char buf[BUFSIZE];
    pid_t pid;

    signal(SIGINT, sig_handler);

    // 创建两个管道
    if (pipe(data_pipe) || pipe(sync_pipe)) {
        perror("pipe");
        return -1;
    }

    // 打开文件
    if ((fp = fopen(FILENAME, "a+")) == NULL) {
        perror("fopen");
        return -1;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid > 0) { // 父进程
        close(data_pipe[0]);  // 关闭数据管道的读端
        close(sync_pipe[1]);  // 关闭同步管道的写端

        while (!flag) {
            printf("Input >");
            fflush(stdout);  // 强制刷新输出缓冲区

            if (fgets(buf, BUFSIZE, stdin)) {
                if (strcmp(buf, "exit\n") == 0) {
                    flag = 1;
                    break;
                }

                // 写入数据到文件
                fputs(buf, fp);
                fflush(fp);  // 刷新文件缓冲区

                // 写入数据到子进程
                write(data_pipe[1], buf, strlen(buf)+1);
                
                // 等待子进程的同步信号
                char sync_byte;
                read(sync_pipe[0], &sync_byte, 1);  // 阻塞直到收到确认
            }
        }

        // 清理资源
        fputs(" --by CDUT\n", fp);
        fclose(fp);
        close(data_pipe[1]);
        close(sync_pipe[0]);
        wait(NULL);  // 等待子进程退出
        exit(0);

    } else { // 子进程
        close(data_pipe[1]);  // 关闭数据管道的写端
        close(sync_pipe[0]);  // 关闭同步管道的读端

        while (!flag) {
            ssize_t n = read(data_pipe[0], buf, BUFSIZE);
            if (n > 0) {
                printf("Received: %s", buf);
                fflush(stdout);  // 确保立即输出

                // 发送同步信号给父进程
                write(sync_pipe[1], "A", 1);  // 任意1字节作为确认
            } else if (n == 0) {
                break;  // 父进程已关闭写端
            }
        }

        close(data_pipe[0]);
        close(sync_pipe[1]);
        exit(0);
    }

    return 0;
}
