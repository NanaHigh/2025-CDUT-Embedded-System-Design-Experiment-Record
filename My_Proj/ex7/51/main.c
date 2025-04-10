#include <reg51.h>
#include <stdio.h>

#define BUF_START 0x40
#define BUF_END   0x7F

void main(void) { 
    // 串口初始化
    SCON = 0x50;    // 模式1，允许接收(REN=1)
    
    // 定时器1初始化（波特率发生器）
    TMOD = 0x20;    // 定时器1模式2（8位自动重装）
    TH1 = 0xFD;     // 波特率9600（11.0592MHz晶振）
    TL1 = 0xFD;
    PCON = 0X00;
       
    // 中断系统
    ES = 1;         // 开启串口中断
    EA = 1;         // 开启总中断
    TR1 = 1;        // 启动定时器1

    
    while (1);      // 主循环保持空转
}

void Receive() interrupt 4 {
    static unsigned char idata *ptr = (unsigned char idata *)BUF_START;
    
    if (RI) { 
        *ptr++ = SBUF;
        // 循环缓冲处理
        if ((unsigned int)ptr > BUF_END) {
            ptr = (unsigned char idata *)BUF_START;
        }
		 RI = 0;
    }
    
    if (TI) {
        TI = 0;              // 清除发送标志
    }
}