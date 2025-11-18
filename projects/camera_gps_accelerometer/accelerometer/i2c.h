#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C1_Init(void);
void I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data);
void I2C_ReadMulti(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len);

#endif

