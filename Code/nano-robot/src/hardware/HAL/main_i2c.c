#define I2C1                (0x40000000U + 0x5400)
#include "stm32f0xx_hal_i2c.h"

I2C_HandleTypeDef hi2c1;

int init_i2c_HAL(uint16_t DevAddress) {
    hi2c1.Instance = I2C1;
    hi2c1.Lock  = HAL_UNLOCKED;
    hi2c1.State = HAL_I2C_STATE_READY;
}

void i2c_HAL_send(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, 100);
}

void i2c_HAL_read(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Master_Receive (&hi2c1, DevAddress, pData, Size, 100);
}

void i2c_HAL_write_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Mem_Write(&hi2c1, DevAddress,
        MemAddress, I2C_MEMADD_SIZE_8BIT,
         pData, Size, 100);
}

void i2c_HAL_read_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Mem_Read(&hi2c1, DevAddress,
        MemAddress, I2C_MEMADD_SIZE_8BIT,
        pData, Size, 100);
}

