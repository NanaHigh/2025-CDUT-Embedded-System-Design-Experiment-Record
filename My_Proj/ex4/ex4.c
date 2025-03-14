#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <linux/input.h>

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

int main(int argc, char *argv[]) {
    uint8_t led_val = 0x01; // 初始值为0x01，即LED1亮

    while (1) {
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

    return 0;
}