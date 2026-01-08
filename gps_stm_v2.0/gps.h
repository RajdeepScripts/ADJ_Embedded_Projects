#ifndef GPS_H
#define GPS_H

#include <stdint.h>

void gps_usart1_init(void);
void gps_poll(void); 


extern volatile char gps_buffer[128];
extern volatile uint8_t gps_sentence_ready;

#endif

