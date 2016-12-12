
// Includes ------------------------------------------------------------------
#define HAL_I2C_MODULE_ENABLED

#include "stm32f0xx_hal_i2c.h"

/******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_CR2_SADD_Pos             (0U)
#define I2C_CR2_SADD_Msk             (0x3FFU << I2C_CR2_SADD_Pos)              /*!< 0x000003FF */
#define I2C_CR2_SADD                 I2C_CR2_SADD_Msk                          /*!< Slave address (master mode) */
#define I2C_CR2_RD_WRN_Pos           (10U)
#define I2C_CR2_RD_WRN_Msk           (0x1U << I2C_CR2_RD_WRN_Pos)              /*!< 0x00000400 */
#define I2C_CR2_RD_WRN               I2C_CR2_RD_WRN_Msk                        /*!< Transfer direction (master mode) */
#define I2C_CR2_ADD10_Pos            (11U)
#define I2C_CR2_ADD10_Msk            (0x1U << I2C_CR2_ADD10_Pos)               /*!< 0x00000800 */
#define I2C_CR2_ADD10                I2C_CR2_ADD10_Msk                         /*!< 10-bit addressing mode (master mode) */
#define I2C_CR2_HEAD10R_Pos          (12U)
#define I2C_CR2_HEAD10R_Msk          (0x1U << I2C_CR2_HEAD10R_Pos)             /*!< 0x00001000 */
#define I2C_CR2_HEAD10R              I2C_CR2_HEAD10R_Msk                       /*!< 10-bit address header only read direction (master mode) */
#define I2C_CR2_START_Pos            (13U)
#define I2C_CR2_START_Msk            (0x1U << I2C_CR2_START_Pos)               /*!< 0x00002000 */
#define I2C_CR2_START                I2C_CR2_START_Msk                         /*!< START generation */
#define I2C_CR2_STOP_Pos             (14U)
#define I2C_CR2_STOP_Msk             (0x1U << I2C_CR2_STOP_Pos)                /*!< 0x00004000 */
#define I2C_CR2_STOP                 I2C_CR2_STOP_Msk                          /*!< STOP generation (master mode) */
#define I2C_CR2_NACK_Pos             (15U)
#define I2C_CR2_NACK_Msk             (0x1U << I2C_CR2_NACK_Pos)                /*!< 0x00008000 */
#define I2C_CR2_NACK                 I2C_CR2_NACK_Msk                          /*!< NACK generation (slave mode) */
#define I2C_CR2_NBYTES_Pos           (16U)
#define I2C_CR2_NBYTES_Msk           (0xFFU << I2C_CR2_NBYTES_Pos)             /*!< 0x00FF0000 */
#define I2C_CR2_NBYTES               I2C_CR2_NBYTES_Msk                        /*!< Number of bytes */
#define I2C_CR2_RELOAD_Pos           (24U)
#define I2C_CR2_RELOAD_Msk           (0x1U << I2C_CR2_RELOAD_Pos)              /*!< 0x01000000 */
#define I2C_CR2_RELOAD               I2C_CR2_RELOAD_Msk                        /*!< NBYTES reload mode */
#define I2C_CR2_AUTOEND_Pos          (25U)
#define I2C_CR2_AUTOEND_Msk          (0x1U << I2C_CR2_AUTOEND_Pos)             /*!< 0x02000000 */
#define I2C_CR2_AUTOEND              I2C_CR2_AUTOEND_Msk                       /*!< Automatic end mode (master mode) */

/******************  Bit definition for I2C_ISR register  ********************/
#define I2C_ISR_TXE_Pos              (0U)
#define I2C_ISR_TXE_Msk              (0x1U << I2C_ISR_TXE_Pos)                 /*!< 0x00000001 */
#define I2C_ISR_TXE                  I2C_ISR_TXE_Msk                           /*!< Transmit data register empty */
#define I2C_ISR_TXIS_Pos             (1U)
#define I2C_ISR_TXIS_Msk             (0x1U << I2C_ISR_TXIS_Pos)                /*!< 0x00000002 */
#define I2C_ISR_TXIS                 I2C_ISR_TXIS_Msk                          /*!< Transmit interrupt status */
#define I2C_ISR_RXNE_Pos             (2U)
#define I2C_ISR_RXNE_Msk             (0x1U << I2C_ISR_RXNE_Pos)                /*!< 0x00000004 */
#define I2C_ISR_RXNE                 I2C_ISR_RXNE_Msk                          /*!< Receive data register not empty */
#define I2C_ISR_ADDR_Pos             (3U)
#define I2C_ISR_ADDR_Msk             (0x1U << I2C_ISR_ADDR_Pos)                /*!< 0x00000008 */
#define I2C_ISR_ADDR                 I2C_ISR_ADDR_Msk                          /*!< Address matched (slave mode)*/
#define I2C_ISR_NACKF_Pos            (4U)
#define I2C_ISR_NACKF_Msk            (0x1U << I2C_ISR_NACKF_Pos)               /*!< 0x00000010 */
#define I2C_ISR_NACKF                I2C_ISR_NACKF_Msk                         /*!< NACK received flag */
#define I2C_ISR_STOPF_Pos            (5U)
#define I2C_ISR_STOPF_Msk            (0x1U << I2C_ISR_STOPF_Pos)               /*!< 0x00000020 */
#define I2C_ISR_STOPF                I2C_ISR_STOPF_Msk                         /*!< STOP detection flag */
#define I2C_ISR_TC_Pos               (6U)
#define I2C_ISR_TC_Msk               (0x1U << I2C_ISR_TC_Pos)                  /*!< 0x00000040 */
#define I2C_ISR_TC                   I2C_ISR_TC_Msk                            /*!< Transfer complete (master mode) */
#define I2C_ISR_TCR_Pos              (7U)
#define I2C_ISR_TCR_Msk              (0x1U << I2C_ISR_TCR_Pos)                 /*!< 0x00000080 */
#define I2C_ISR_TCR                  I2C_ISR_TCR_Msk                           /*!< Transfer complete reload */
#define I2C_ISR_BUSY_Pos             (15U)
#define I2C_ISR_BUSY_Msk             (0x1U << I2C_ISR_BUSY_Pos)                /*!< 0x00008000 */
#define I2C_ISR_BUSY                 I2C_ISR_BUSY_Msk                          /*!< Bus busy */


#define HAL_I2C_ERROR_NONE      (0x00000000U)    /*!< No error              */
#define HAL_I2C_ERROR_AF        (0x00000004U)    /*!< ACKF error            */
#define HAL_I2C_ERROR_TIMEOUT   (0x00000020U)    /*!< Timeout error         */


#define I2C_RELOAD_MODE                 I2C_CR2_RELOAD
#define I2C_AUTOEND_MODE                I2C_CR2_AUTOEND
#define I2C_SOFTEND_MODE                (0x00000000U)

#define I2C_NO_STARTSTOP                (0x00000000U)
#define I2C_GENERATE_STOP               I2C_CR2_STOP
#define I2C_GENERATE_START_READ         (uint32_t)(I2C_CR2_START | I2C_CR2_RD_WRN)
#define I2C_GENERATE_START_WRITE        I2C_CR2_START

#define I2C_FLAG_TXE                    I2C_ISR_TXE
#define I2C_FLAG_TXIS                   I2C_ISR_TXIS
#define I2C_FLAG_RXNE                   I2C_ISR_RXNE
#define I2C_FLAG_ADDR                   I2C_ISR_ADDR
#define I2C_FLAG_AF                     I2C_ISR_NACKF
#define I2C_FLAG_STOPF                  I2C_ISR_STOPF
#define I2C_FLAG_TC                     I2C_ISR_TC
#define I2C_FLAG_TCR                    I2C_ISR_TCR
#define I2C_FLAG_BUSY                   I2C_ISR_BUSY


#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->ISR) & (__FLAG__)) == (__FLAG__)) ? SET : RESET)

#define __HAL_I2C_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__FLAG__) == I2C_FLAG_TXE) ? ((__HANDLE__)->ISR |= (__FLAG__)) \
                                                                                 : ((__HANDLE__)->ICR = (__FLAG__)))


#define I2C_RESET_CR2(__HANDLE__)                 ((__HANDLE__)->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN)))


#define I2C_MEM_ADD_MSB(__ADDRESS__)              ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)(0xFF00U))) >> 8U)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)              ((uint8_t)((uint16_t)(            (__ADDRESS__) & (uint16_t)(0x00FFU)))        )

#define I2C_GENERATE_START(__ADDMODE__,__ADDRESS__) (((__ADDMODE__) == I2C_ADDRESSINGMODE_7BIT) ? (uint32_t)((((uint32_t)(__ADDRESS__) & (I2C_CR2_SADD)) | (I2C_CR2_START) | (I2C_CR2_AUTOEND)) & (~I2C_CR2_RD_WRN)) : \


#define TIMING_CLEAR_MASK   (0xF0FFFFFFU)  //!< I2C TIMING clear register Mask
#define I2C_TIMEOUT_BUSY    (25U)          //!< 25 ms
#define MAX_NBYTE_SIZE      255U


static void I2C_Flush_TXDR(I2C_HandleTypeDef *hi2c) {
    // If a pending TXIS flag is set
    // Write a dummy data in TXDR to clear it
    if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) != RESET) {
        hi2c->TXDR = 0x00U;
    }

    // Flush TX register if not empty
    if(__HAL_I2C_GET_FLAG (hi2c, I2C_FLAG_TXE) == RESET) {
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_TXE);
    }
}

static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c) {
    if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET) {
        // Wait until STOP Flag is reset
        // AutoEnd should be initiate after AF
        while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET) {
            // Check for the Timeout
        }

        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

        I2C_Flush_TXDR(hi2c);

        I2C_RESET_CR2(hi2c);

        return HAL_ACKFAIL;
    }
    return HAL_OK;
}

static void I2C_WaitOnFlag(I2C_HandleTypeDef *hi2c,
    uint32_t Flag,
    FlagStatus Status) {
    while(__HAL_I2C_GET_FLAG(hi2c, Flag) == Status) {
    }
}

static HAL_StatusTypeDef I2C_WaitOnTXISFlag(I2C_HandleTypeDef *hi2c) {
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) == RESET) {
        // Check if a NACK is detected
        if(I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
            return HAL_ACKFAIL;
    }
    return HAL_OK;
}

static HAL_StatusTypeDef I2C_WaitOnSTOPFlag(I2C_HandleTypeDef *hi2c) {
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET) {
        // Check if a NACK is detected
        if(I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
            return HAL_ACKFAIL;
    }
    return HAL_OK;
}

static HAL_StatusTypeDef I2C_WaitOnRXNEFlag(I2C_HandleTypeDef *hi2c) {
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET) {
        // // Check if a NACK is detected
        if(I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
            return HAL_ACKFAIL;

        // Check if a STOPF is detected
        if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET) {
            // Clear STOP Flag
            __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

            // Clear Configuration Register 2
            I2C_RESET_CR2(hi2c);

            return HAL_ERROR;
        }
    }
    return HAL_OK;
}

static void I2C_TransferConfig(I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint8_t  Size,
    uint32_t Mode,
    uint32_t Request) {
    uint32_t tmpreg = 0U;

    // Get the CR2 register value
    tmpreg = hi2c->CR2;

    // clear tmpreg specific bits
    tmpreg &= ~((uint32_t)(
          I2C_CR2_SADD
        | I2C_CR2_NBYTES
        | I2C_CR2_RD_WRN
        | I2C_CR2_START
        | I2C_CR2_STOP
        | I2C_CR2_RELOAD
        | I2C_CR2_AUTOEND
        ));

    // update tmpreg
    tmpreg |=( ((uint32_t)DevAddress   & I2C_CR2_SADD)
            | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES)
            | Mode
            | Request
            );

    // update CR2 register
    hi2c->CR2 = tmpreg;
}


HAL_StatusTypeDef HAL_I2C_Master_Transmit   (I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint8_t *pData,
    uint16_t Size) {

    I2C_WaitOnFlag(hi2c, I2C_FLAG_BUSY, SET);

    // Prepare transfer parameters
    uint16_t XferTotal = Size;
    uint16_t XferSize = 0U;

    // Send Slave Address
    // Set NBYTES to write and reload if XferTotal > MAX_NBYTE_SIZE and generate RESTART
    if(XferTotal > MAX_NBYTE_SIZE) {
        XferSize = MAX_NBYTE_SIZE;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE,  I2C_GENERATE_START_WRITE);
    } else {
        XferSize = XferTotal;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);
    }

    while(XferTotal > 0U) {
        // Wait until TXIS flag is set
        switch(I2C_WaitOnTXISFlag(hi2c)) {
            case HAL_OK:        break;
            case HAL_ACKFAIL:   return HAL_ACKFAIL;
            case HAL_ERROR:     return HAL_ERROR;
        }
        // Write data to TXDR
        hi2c->TXDR = (*pData++);
        XferTotal--;
        XferSize--;

        if((XferSize == 0U) && (XferTotal!=0U)) {
            // Wait until TCR flag is set
            I2C_WaitOnFlag(hi2c, I2C_FLAG_TCR, RESET);

            if(XferTotal > MAX_NBYTE_SIZE) {
                XferSize = MAX_NBYTE_SIZE;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
            } else {
                XferSize = XferTotal;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
            }
        }
    }

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is set
    switch(I2C_WaitOnSTOPFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive    (I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint8_t *pData,
    uint16_t Size) {

    I2C_WaitOnFlag(hi2c, I2C_FLAG_BUSY, SET);

    // Prepare transfer parameters
    uint16_t XferTotal = Size;
    uint16_t XferSize;

    // Send Slave Address
    // Set NBYTES to write and reload if XferTotal > MAX_NBYTE_SIZE and generate RESTART
    if(XferTotal > MAX_NBYTE_SIZE) {
        XferSize = MAX_NBYTE_SIZE;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_READ);
    } else {
        XferSize = XferTotal;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);
    }

    while(XferTotal > 0U) {
        // Wait until RXNE flag is set
        switch(I2C_WaitOnRXNEFlag(hi2c)) {
            case HAL_OK:        break;
            case HAL_ACKFAIL:   return HAL_ACKFAIL;
            case HAL_ERROR:     return HAL_ERROR;
        }

        // Read data from RXDR
        (*pData++) = hi2c->RXDR;
        XferSize--;
        XferTotal--;

        if((XferSize == 0U) && (XferTotal != 0U)) {
            // Wait until TCR flag is set
            I2C_WaitOnFlag(hi2c, I2C_FLAG_TCR, RESET);

            if(XferTotal > MAX_NBYTE_SIZE) {
                XferSize = MAX_NBYTE_SIZE;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
            } else {
                XferSize = XferTotal;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
            }
        }
    }

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is set
    switch(I2C_WaitOnSTOPFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write         (I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size) {

    I2C_WaitOnFlag(hi2c, I2C_FLAG_BUSY, SET);

    // Prepare transfer parameters
    uint16_t XferTotal = Size;
    uint16_t XferSize;

    //// Send Slave Address and Memory Address
    I2C_TransferConfig(hi2c, DevAddress, MemAddSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);
    // Wait until TXIS flag is set
    switch(I2C_WaitOnTXISFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    hi2c->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    // Wait until TCR flag is set
    I2C_WaitOnFlag(hi2c, I2C_FLAG_TCR, RESET);


    // Set NBYTES to write and reload if XferTotal > MAX_NBYTE_SIZE
    if(XferTotal > MAX_NBYTE_SIZE) {
        XferSize = MAX_NBYTE_SIZE;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
    } else {
        XferSize = XferTotal;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
    }

    do {
        // Wait until TXIS flag is set
        switch(I2C_WaitOnTXISFlag(hi2c)) {
            case HAL_OK:        break;
            case HAL_ACKFAIL:   return HAL_ACKFAIL;
            case HAL_ERROR:     return HAL_ERROR;
        }

        // Write data to TXDR
        hi2c->TXDR = (*pData++);
        XferTotal--;
        XferSize--;

        if((XferSize == 0U) && (XferTotal!=0U)) {
            // Wait until TCR flag is set
            I2C_WaitOnFlag(hi2c, I2C_FLAG_TCR, RESET);

            if(XferTotal > MAX_NBYTE_SIZE) {
                XferSize = MAX_NBYTE_SIZE;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
            } else {
                XferSize = XferTotal;
                I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
            }
        }

    } while(XferTotal > 0U);

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is reset
    switch(I2C_WaitOnSTOPFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read          (I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size) {

    I2C_WaitOnFlag(hi2c, I2C_FLAG_BUSY, SET);

    // Prepare transfer parameters
    uint16_t XferTotal = Size;
    uint16_t XferSize;

    //// Send Slave Address and Memory Address
    I2C_TransferConfig(hi2c,DevAddress,MemAddSize, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
    // Wait until TXIS flag is set
    switch(I2C_WaitOnTXISFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    hi2c->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    // Wait until TC flag is set
    I2C_WaitOnFlag(hi2c, I2C_FLAG_TC, RESET);


    // Send Slave Address
    // Set NBYTES to write and reload if XferTotal > MAX_NBYTE_SIZE and generate RESTART
    if(XferTotal > MAX_NBYTE_SIZE) {
        XferSize = MAX_NBYTE_SIZE;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_READ);
    } else {
        XferSize = XferTotal;
        I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);
    }

    do {
        // Wait until RXNE flag is set
        I2C_WaitOnFlag(hi2c, I2C_FLAG_RXNE, RESET);

        // Read data from RXDR
        (*pData++) = hi2c->RXDR;
        XferSize--;
        XferTotal--;

        if((XferSize == 0U) && (XferTotal != 0U)) {
            // Wait until TCR flag is set
            I2C_WaitOnFlag(hi2c, I2C_FLAG_TCR, RESET);

            XferSize = XferTotal;
            I2C_TransferConfig(hi2c, DevAddress, XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        }
    } while(XferTotal > 0U);

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is reset
    switch(I2C_WaitOnSTOPFlag(hi2c)) {
        case HAL_OK:        break;
        case HAL_ACKFAIL:   return HAL_ACKFAIL;
        case HAL_ERROR:     return HAL_ERROR;
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    return HAL_OK;
}

