#include "stm32f4xx.h"
#include "uart.h"

// Initialize USART2 for UART communication
void UART2_Init(void) {
    // Enable GPIOA and USART2 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 as USART2 TX and PA3 as RX (Alternate function 7)
    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1); // AF mode
    GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3)));
    GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));

    // Configure USART2 parameters: 9600 baud, 8N1
    USART2->BRR = 16000000 / 115200;  // Assuming 16 MHz APB1 clock
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;  // Enable TX, RX
    USART2->CR2 = 0;  // 1 stop bit
    USART2->CR3 = 0;
    USART2->CR1 |= USART_CR1_UE;  // Enable USART2
}

// Send a character over USART2
void UART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

// Send a null-terminated string over USART2
void UART2_SendString(const char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

