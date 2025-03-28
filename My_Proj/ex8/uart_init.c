#include <termios.h>
#include <string.h>
#include <stdio.h>
#include "uart_init.h"

int uart_init(int fd, int nSpeed, int nBits, char nEvent, int nStop) {
    struct termios newtio, oldtio;
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("tcgetattr");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits) {
        case 7: newtio.c_cflag |= CS7; break;
        case 8: newtio.c_cflag |= CS8; break;
    }

    switch (nEvent) {
        case 'O': // 奇校验
            newtio.c_cflag |= PARENB | PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E': // 偶校验
            newtio.c_cflag |= PARENB;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'N': // 无校验
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch (nSpeed) {
        case 2400:   cfsetispeed(&newtio, B2400); break;
        case 4800:   cfsetispeed(&newtio, B4800); break;
        case 9600:   cfsetispeed(&newtio, B9600); break;
        case 115200: cfsetispeed(&newtio, B115200); break;
        default:     cfsetispeed(&newtio, B9600); break;
    }
    cfsetospeed(&newtio, cfgetispeed(&newtio));

    if (nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if (nStop == 2)
        newtio.c_cflag |= CSTOPB;

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &newtio) != 0) {
        perror("tcsetattr");
        return -1;
    }
    return 0;
}