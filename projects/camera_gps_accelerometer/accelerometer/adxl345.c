#include "adxl345.h"
#include "i2c.h"

#define ADXL345_ADDR   (0x53 << 1)

void ADXL345_Init(void)
{
    I2C_WriteReg(ADXL345_ADDR, 0x2D, 0x08);  // POWER_CTL: measure = 1
    I2C_WriteReg(ADXL345_ADDR, 0x31, 0x08);  // DATA_FORMAT: full res, ±2g
    I2C_WriteReg(ADXL345_ADDR, 0x2C, 0x0A);  // BW_RATE: 100 Hz
}

void ADXL345_ReadXYZ(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];
    I2C_ReadMulti(ADXL345_ADDR, 0x32, data, 6);

    *x = (int16_t)((data[1] << 8) | data[0]);
    *y = (int16_t)((data[3] << 8) | data[2]);
    *z = (int16_t)((data[5] << 8) | data[4]);
}

