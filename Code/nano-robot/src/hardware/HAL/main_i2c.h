#pragma once

#include <stdint.h>

void i2c_HAL_send(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void i2c_HAL_read(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void i2c_HAL_write_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size);
void i2c_HAL_read_register
                 (uint16_t DevAddress,
                  uint16_t MemAddress, uint8_t *pData, uint16_t Size);
