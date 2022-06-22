#include <wiringPi.h>
#include <stdio.h>
#include <wiringPiI2C.h>
#include <stdint.h>

#define SENSOR_ADDR 0x23
#define SENSOR_READ_HIRES_CMD 0b00100000

void send_command(int fd, int cmd)
{
    wiringPiI2CWrite(fd, cmd);
}

float get_lux(int fd)
{
    int msb = wiringPiI2CRead(fd);
    int lsb = wiringPiI2CRead(fd);

    uint16_t raw = msb << 8 | lsb;
    float result = (float)raw / 1.2f;

    return result;
}

int main()
{
    int fd = wiringPiI2CSetup(SENSOR_ADDR);
    if (fd == -1)
    {
        printf("Failed setup I2C.\r\n");

        return -1;
    }

    for (;;)
    {
        send_command(fd, SENSOR_READ_HIRES_CMD);
        delay(200);

        float lux = get_lux(fd);
        printf("Light: %.2f lx\r\n", lux);

        delay(1000);
    }

    return 0;
}
