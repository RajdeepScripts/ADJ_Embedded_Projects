#include "gps.h"
#include "stm32f411xe.h"
#include "usart_debug.h"


/* Buffer to store one complete NMEA sentence received from GPS.
  Marked volatile because it is modified inside an ISR (USART RX). */
volatile char gps_buffer[128];

/* Flag set when a full GPS sentence (ending with '\n') is received.
  Main loop checks this flag to process the sentence. */

volatile uint8_t gps_sentence_ready = 0;

/* Index used to track current write position in gps_buffer.
  Static limits its visibility to this file only.
  Volatile because it is updated from interrupt context. */
								 

static volatile uint8_t gps_idx = 0;

/* Configures:
   - GPIOA pins PA9 (TX) and PA10 (RX) in alternate function mode
   - USART1 peripheral with required baud rate
   - Receiver enabled (polling / interrupt handled elsewhere) */

void gps_usart1_init(void)
{
   
    // Enable GPIOA clock (PA9, PA10)   
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // Enable USART1 peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // PA9 = TX, PA10 = RX
    GPIOA->MODER &= ~((3 << (9*2)) | (3 << (10*2)));
    GPIOA->MODER |=  ((2 << (9*2)) | (2 << (10*2)));
    //Set PA9 and PA10 to Alternate Function mode and AF7 (for USART1)
    GPIOA->AFR[1] &= ~((0xF << 4) | (0xF << 8));
    GPIOA->AFR[1] |=  ((7 << 4) | (7 << 8));   // AF7

    uint32_t baud = 9600;   // Depends on the gps but for now the gps we are using baud rate is 9600
    USART1->BRR = (16000000U + baud/2) / baud; //APB2 clock is 16MHz

    USART1->CR1 = USART_CR1_RE | USART_CR1_UE;  // Enabling USART receiver and USART peripheral we are not enabling transmission because GPS only sends the data
}


/**
 * @brief Poll USART1 and assemble GPS NMEA sentences.
 *
 * This function:
 *  - Polls the USART RXNE flag (non-interrupt based reception)
 *  - Reads incoming characters one by one
 *  - Buffers characters until a newline ('\n') is received
 *  - Prints only valid NMEA sentences (starting with '$')
 *
 * This function is intended to be called periodically
 * from the main loop.
 */
void gps_poll(void)
{
    /* Local static buffer to hold one NMEA sentence */
    static char line[128];

    /* Index to track current write position in the buffer */
    static uint8_t idx = 0;

    /* Check if a new character has been received */
    if (USART1->SR & USART_SR_RXNE)
    {
        /* Reading DR clears RXNE flag */
        char c = USART1->DR;

        /* NMEA sentences are terminated by newline character */
        if (c == '\n')
        {
            /* Null-terminate the string */
            line[idx] = '\0';

            /* Print only valid NMEA sentences */
            if (line[0] == '$')
            {
                usart_debug("%s\r\n", line);
            }

            /* Reset buffer index for next sentence */
            idx = 0;
        }
        else
        {
            /* Store character if buffer space is available */
            if (idx < (sizeof(line) - 1U))
            {
                line[idx++] = c;
            }
            else
            {
                /* Buffer overflow protection:
                 * discard current sentence and restart
                 */
                idx = 0;
            }
        }
    }
}

