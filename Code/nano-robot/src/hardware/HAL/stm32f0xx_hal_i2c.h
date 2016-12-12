#pragma once

#include <stdint.h>
#include <stdio.h>

#define     __IO    volatile             /*!< Defines 'read / write' permissions */

typedef enum {
    RESET = 0,
    SET = !RESET
} FlagStatus;

typedef enum {
    HAL_OK       = 0x00,
    HAL_ERROR    = 0x01,
    HAL_ACKFAIL  = 0x04
} HAL_StatusTypeDef;

typedef struct {
    __IO uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
    __IO uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
    __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
    __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
    __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
    __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
    __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
    __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
    __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
    __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
    __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_HandleTypeDef;


HAL_StatusTypeDef HAL_I2C_Master_Transmit
        (I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive
        (I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2C_Mem_Write
        (I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Read
        (I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
