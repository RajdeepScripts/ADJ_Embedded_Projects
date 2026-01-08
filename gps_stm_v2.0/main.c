#include "stm32f4xx.h"
#include "gps.h"
#include "usart_debug.h"

int main(void)
{
    USART2_Init();
    usart_debug("BOOT OK\r\n");

    gps_usart1_init();
    usart_debug("GPS UART READY\r\n");

    while (1)
    {
        gps_poll(); //in gps.c  
    }
}

