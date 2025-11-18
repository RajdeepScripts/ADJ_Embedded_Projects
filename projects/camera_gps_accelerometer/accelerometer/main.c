#include "stm32f4xx.h"
#include "i2c.h"
#include "adxl345.h"
#include "uart.h"
#include <stdio.h>

int16_t x, y, z;

int main(void)
{
    I2C1_Init();
    UART2_Init();
    ADXL345_Init();

    UART2_SendString("System Started\r\n");

    while (1)
    {
        ADXL345_ReadXYZ(&x, &y, &z);

        char buf[50];
        sprintf(buf, "X=%d Y=%d Z=%d\r\n", x, y, z);
        UART2_SendString(buf);

        for (volatile int i = 0; i < 100000; i++);
    }
}

