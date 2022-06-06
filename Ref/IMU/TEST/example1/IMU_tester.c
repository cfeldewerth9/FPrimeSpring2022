#include <stdio.h>
#include <wiringPiI2C.h>
#include <errno.h>

int main() {
    int returnVal;
    int fd;

    fd = wiringPiI2CSetup(0x6A);
    printf("%d\n", fd);
    return 0;
}