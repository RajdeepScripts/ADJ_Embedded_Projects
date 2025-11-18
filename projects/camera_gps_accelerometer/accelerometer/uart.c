#include "uart.h"

void UART2_Init(void)
{
    // Enable clocks
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA2 = TX, PA3 = RX (AF7)
    GPIOA->MODER &= ~(0xF << 4);
    GPIOA->MODER |= (0xA << 4); // PA2/PA3 alternate function

    GPIOA->AFR[0] |= (7 << 8) | (7 << 12); // AF7 for USART2

    GPIOA->OSPEEDR |= (0xF << 4);

    USART2->BRR = 0x0683; // 115200 baud
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void UART2_SendString(const char *str)
{
    while (*str)
        UART2_SendChar(*str++);
}

