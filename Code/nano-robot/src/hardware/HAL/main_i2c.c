#define I2C1                (0x40000000U + 0x5400)
#include "stm32f0xx_hal_i2c.h"


void i2c_HAL_write_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Mem_Write((I2C_HandleTypeDef *)I2C1, DevAddress,
        MemAddress, 1,
        pData, Size);
}

void i2c_HAL_read_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Mem_Read((I2C_HandleTypeDef *)I2C1, DevAddress,
        MemAddress, 1,
        pData, Size);
}

