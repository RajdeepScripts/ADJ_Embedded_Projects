#include "i2c.h"
#include "stm32f4xx.h"

void I2C1_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER |= (2 << (8 * 2)) | (2 << (9 * 2)); // AF mode
    GPIOB->AFR[1] |= (4 << (0 * 4)) | (4 << (1 * 4)); // AF4 for PB8/9

    I2C1->CR2 = 16;     // 16 MHz PCLK1
    I2C1->CCR = 80;     // Standard mode
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = addr;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    I2C1->DR = reg;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_ReadMulti(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len)
{
    // write register address
    I2C_WriteReg(addr, reg, 0);

    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = addr | 1;     // read mode
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    for (int i = 0; i < len; i++) {
        if (i == len - 1)
            I2C1->CR1 &= ~I2C_CR1_ACK;  // NACK last byte

        while(!(I2C1->SR1 & I2C_SR1_RXNE));
        buf[i] = I2C1->DR;
    }

    I2C1->CR1 |= I2C_CR1_STOP;
}

