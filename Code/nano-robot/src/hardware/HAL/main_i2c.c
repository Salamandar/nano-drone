#include "stm32f0xx_hal.h"

#include "../leds.h"

I2C_HandleTypeDef hi2c1;

int init_i2c_HAL(uint16_t DevAddress) {
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x0000020B;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
    HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
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

