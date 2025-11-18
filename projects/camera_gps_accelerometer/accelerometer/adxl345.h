#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>

void ADXL345_Init(void);
void ADXL345_ReadXYZ(int16_t *x, int16_t *y, int16_t *z);

#endif
