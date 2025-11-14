#include "stm32f4xx.h"

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 1000; i++)   // Approximate delay
    {
        __NOP();   // Single instruction "No Operation" to waste CPU cycles
    }
}
int main(void)
{
    // Enable GPIOA clock (bit 0 in RCC AHB1ENR)
    RCC->AHB1ENR |= (1 << 0);

    // Set PA5 (pin 5) as output (bit 10 = 1)
    GPIOA->MODER |= (1 << 10) | (1 << 12);

    while (1)
    {
        GPIOA->ODR |= (1 << 5) | (1 << 6);   // LED ON
       // for (int i = 0; i < 1000000; i++);  // Delay
       delay_ms(1000);
        GPIOA->ODR &= ~((1 << 5) | (1 << 6));  // LED OFF
       // for (int i = 0; i < 1000000; i++);
       delay_ms(1000);
    }
}

