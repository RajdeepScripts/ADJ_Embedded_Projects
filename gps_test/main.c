#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RS 0
#define EN 1
#define D4 5
#define D5 6
#define D6 7
#define D7 8

char gps_buffer[200];
int index_gps = 0;

char latitude[20] = "0.000000";
char longitude[20] = "0.000000";

// -------------------- LCD DELAY --------------------
void delay(uint32_t t)
{
    while(t--);
}

// -------------------- LCD FUNCTIONS --------------------
void lcd_pulse()
{
    GPIOB->ODR |= (1<<EN);
    delay(2000);
    GPIOB->ODR &= ~(1<<EN);
    delay(2000);
}

void lcd_send_nibble(uint8_t data)
{
    GPIOB->ODR &= ~((1<<D4)|(1<<D5)|(1<<D6)|(1<<D7));

    if (data & 0x01) GPIOB->ODR |= (1<<D4);
    if (data & 0x02) GPIOB->ODR |= (1<<D5);
    if (data & 0x04) GPIOB->ODR |= (1<<D6);
    if (data & 0x08) GPIOB->ODR |= (1<<D7);

    lcd_pulse();
}

void lcd_cmd(uint8_t cmd)
{
    GPIOB->ODR &= ~(1<<RS);

    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
}

void lcd_data(char data)
{
    GPIOB->ODR |= (1<<RS);

    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
}

void lcd_init()
{
    delay(500000);
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_print(char *str)
{
    while(*str) lcd_data(*str++);
}

// -------------------- GPS PARSER --------------------
void parse_gprmc(char *nmea)
{
    char *token;
    int field = 0;

    token = strtok(nmea, ",");

    while(token != NULL)
    {
        field++;

        if(field == 4) // latitude raw
        {
            float raw = atof(token); // DDMM.MMMM
            int deg = (int)(raw / 100);
            float min = raw - (deg * 100);
            float dec = deg + (min / 60.0);
            sprintf(latitude, "%f", dec);
        }

        if(field == 6) // longitude raw
        {
            float raw = atof(token);
            int deg = (int)(raw / 100);
            float min = raw - (deg * 100);
            float dec = deg + (min / 60.0);
            sprintf(longitude, "%f", dec);
        }

        token = strtok(NULL, ",");
    }
}

// -------------------- UART2 INIT --------------------
void UART2_Init(void)
{
    RCC->AHB1ENR |= (1<<0);
    RCC->APB1ENR |= (1<<17);

    GPIOA->MODER |= (2<<(2*2)) | (2<<(3*2));

    GPIOA->AFR[0] |= (7<<(4*2)) | (7<<(4*3));

    USART2->BRR = 0x1117;
    USART2->CR1 |= (1<<13) | (1<<2) | (1<<5);

    NVIC_EnableIRQ(USART2_IRQn);
}

// -------------------- UART INTERRUPT --------------------
void USART2_IRQHandler(void)
{
    if(USART2->SR & (1<<5))  // RXNE
    {
        char c = USART2->DR;

        if(c == '\n')
        {
            gps_buffer[index_gps] = '\0';
            index_gps = 0;

            if(strstr(gps_buffer, "$GPRMC"))
            {
                parse_gprmc(gps_buffer);
            }
        }
        else
        {
            gps_buffer[index_gps++] = c;
            if(index_gps >= 199) index_gps = 0;
        }
    }
}

// -------------------- MAIN --------------------
int main()
{
    RCC->AHB1ENR |= (1<<1);

    GPIOB->MODER |= (1<<(RS*2)) | (1<<(EN*2)) |
                    (1<<(D4*2)) | (1<<(D5*2)) |
                    (1<<(D6*2)) | (1<<(D7*2));

    lcd_init();
    UART2_Init();

    lcd_cmd(0x80);
    lcd_print("GPS Starting...");
    delay(2000000);

    while(1)
    {
        lcd_cmd(0x01);

        lcd_cmd(0x80);
        lcd_print("LAT:");
        lcd_print(latitude);

        lcd_cmd(0xC0);
        lcd_print("LON:");
        lcd_print(longitude);

        delay(5000000);
    }
}

