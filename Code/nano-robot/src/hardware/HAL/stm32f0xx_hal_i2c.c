
// Includes ------------------------------------------------------------------
#define HAL_I2C_MODULE_ENABLED

#include "stm32f0xx_hal_i2c.h"


__IO uint32_t uwTick;
uint32_t HAL_GetTick(void) {
  return uwTick;
}


#define TIMING_CLEAR_MASK   (0xF0FFFFFFU)  //!< I2C TIMING clear register Mask
#define I2C_TIMEOUT_ADDR    (10000U)       //!< 10 s
#define I2C_TIMEOUT_BUSY    (25U)          //!< 25 ms
#define I2C_TIMEOUT_DIR     (25U)          //!< 25 ms
#define I2C_TIMEOUT_RXNE    (25U)          //!< 25 ms
#define I2C_TIMEOUT_STOPF   (25U)          //!< 25 ms
#define I2C_TIMEOUT_TC      (25U)          //!< 25 ms
#define I2C_TIMEOUT_TCR     (25U)          //!< 25 ms
#define I2C_TIMEOUT_TXIS    (25U)          //!< 25 ms
#define I2C_TIMEOUT_FLAG    (25U)          //!< 25 ms

#define MAX_NBYTE_SIZE      255U



  // * @brief  I2C Tx data register flush process.
  // * @param  hi2c I2C handle.
  // * @retval None

static void I2C_Flush_TXDR(I2C_HandleTypeDef *hi2c) {
  // If a pending TXIS flag is set
  // Write a dummy data in TXDR to clear it
  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) != RESET)
  {
     hi2c->Instance->TXDR = 0x00U;
  }

  // Flush TX register if not empty
  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) == RESET)
  {
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_TXE);
  }
}


static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c,
    uint32_t Timeout,
    uint32_t Tickstart) {
    if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET) {
        // Wait until STOP Flag is reset
        // AutoEnd should be initiate after AF
        while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET) {
            // Check for the Timeout
            if(Timeout != HAL_MAX_DELAY) {
                if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout)) {
                    hi2c->State= HAL_I2C_STATE_READY;
                    hi2c->Mode = HAL_I2C_MODE_NONE;

                    // Process Unlocked
                    __HAL_UNLOCK(hi2c);
                    return HAL_TIMEOUT;
                }
            }
        }

        // Clear NACKF Flag
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

        // Clear STOP Flag
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

        // Flush TX register
        I2C_Flush_TXDR(hi2c);

        // Clear Configuration Register 2
        I2C_RESET_CR2(hi2c);

        hi2c->ErrorCode = HAL_I2C_ERROR_AF;
        hi2c->State= HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        // Process Unlocked
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
    }
    return HAL_OK;
}




  // * @brief  This function handles I2C Communication Timeout.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  Flag Specifies the I2C flag to check.
  // * @param  Status The new Flag status (SET or RESET).
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status

static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c,
    uint32_t Flag,
    FlagStatus Status,
    uint32_t Timeout,
    uint32_t Tickstart) {
  while(__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)
  {
    // Check for the Timeout
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U)||((HAL_GetTick() - Tickstart ) > Timeout))
      {
        hi2c->State= HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        // Process Unlocked
        __HAL_UNLOCK(hi2c);
        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}

  // * @brief  This function handles I2C Communication Timeout for specific usage of TXIS flag.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status


static HAL_StatusTypeDef I2C_WaitOnTXISFlagUntilTimeout(
    I2C_HandleTypeDef *hi2c,
    uint32_t Timeout,
    uint32_t Tickstart) {
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) == RESET) {
        // Check if a NACK is detected
        if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK) {
            return HAL_ERROR;
        }

        // Check for the Timeout
        if(Timeout != HAL_MAX_DELAY) {
            if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout)) {
                hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
                hi2c->State= HAL_I2C_STATE_READY;
                hi2c->Mode = HAL_I2C_MODE_NONE;

                // Process Unlocked
                __HAL_UNLOCK(hi2c);

                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}


  // * @brief  This function handles I2C Communication Timeout for specific usage of STOP flag.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status

static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c,
    uint32_t Timeout,
    uint32_t Tickstart) {
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET) {
    // // Check if a NACK is detected
    if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    // Check for the Timeout
    if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout))
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      hi2c->State= HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;

      // Process Unlocked
      __HAL_UNLOCK(hi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

  // * @brief  This function handles I2C Communication Timeout for specific usage of RXNE flag.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status

static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c,
    uint32_t Timeout,
    uint32_t Tickstart) {
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET)
  {
    // // Check if a NACK is detected
    if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    // Check if a STOPF is detected
    if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET)
    {
      // Clear STOP Flag
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

      // Clear Configuration Register 2
      I2C_RESET_CR2(hi2c);

      hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
      hi2c->State= HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;

      // Process Unlocked
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    // Check for the Timeout
    if((Timeout == 0U)||((HAL_GetTick() - Tickstart) > Timeout))
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      hi2c->State= HAL_I2C_STATE_READY;

      // Process Unlocked
      __HAL_UNLOCK(hi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}


  // * @brief  Handles I2Cx communication when starting transfer or during transfer (TC or TCR flag are set).
  // * @param  hi2c I2C handle.
  // * @param  DevAddress Specifies the slave address to be programmed.
  // * @param  Size Specifies the number of bytes to be programmed.
  // *   This parameter must be a value between 0 and 255.
  // * @param  Mode New state of the I2C START condition generation.
  // *   This parameter can be one of the following values:
  // *     @arg @ref I2C_RELOAD_MODE Enable Reload mode .
  // *     @arg @ref I2C_AUTOEND_MODE Enable Automatic end mode.
  // *     @arg @ref I2C_SOFTEND_MODE Enable Software end mode.
  // * @param  Request New state of the I2C START condition generation.
  // *   This parameter can be one of the following values:
  // *     @arg @ref I2C_NO_STARTSTOP Don't Generate stop and start condition.
  // *     @arg @ref I2C_GENERATE_STOP Generate stop condition (Size should be set to 0).
  // *     @arg @ref I2C_GENERATE_START_READ Generate Restart for read request.
  // *     @arg @ref I2C_GENERATE_START_WRITE Generate Restart for write request.
  // * @retval None

static void I2C_TransferConfig(I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint8_t Size,
    uint32_t Mode,
    uint32_t Request) {
  uint32_t tmpreg = 0U;

  // Get the CR2 register value
  tmpreg = hi2c->Instance->CR2;

  // clear tmpreg specific bits
  tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));

  // update tmpreg
  tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES) | \
            (uint32_t)Mode | (uint32_t)Request);

  // update CR2 register
  hi2c->Instance->CR2 = tmpreg;
}




  // * @brief  Master sends target device address followed by internal memory address for write request.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  DevAddress Target device address: The device 7 bits address value
  // *         in datasheet must be shift at right before call interface
  // * @param  MemAddress Internal memory address
  // * @param  MemAddSize Size of internal memory address
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status

static HAL_StatusTypeDef I2C_RequestMemoryWrite(
    I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint32_t Timeout,
    uint32_t Tickstart) {
    I2C_TransferConfig(hi2c,DevAddress,MemAddSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);

    // Wait until TXIS flag is set
    if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK) {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
            return HAL_ERROR;
        else
            return HAL_TIMEOUT;
    }

    // If Memory address size is 8Bit
    if(MemAddSize == I2C_MEMADD_SIZE_8BIT)// Send Memory Address
        hi2c->Instance->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    else { // If Memory address size is 16Bit

        // Send MSB of Memory Address
        hi2c->Instance->TXDR = I2C_MEM_ADD_MSB(MemAddress);

        // Wait until TXIS flag is set
        if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK) {
            if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
                return HAL_ERROR;
            else
                return HAL_TIMEOUT;
        }

        // Send LSB of Memory Address
        hi2c->Instance->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    }

    // Wait until TCR flag is set
    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, Tickstart) != HAL_OK)
        return HAL_TIMEOUT;

    return HAL_OK;
}

  // * @brief  Master sends target device address followed by internal memory address for read request.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  DevAddress Target device address: The device 7 bits address value
  // *         in datasheet must be shift at right before call interface
  // * @param  MemAddress Internal memory address
  // * @param  MemAddSize Size of internal memory address
  // * @param  Timeout Timeout duration
  // * @param  Tickstart Tick start value
  // * @retval HAL status

static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint32_t Timeout,
    uint32_t Tickstart) {
  I2C_TransferConfig(hi2c,DevAddress,MemAddSize, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);

  // Wait until TXIS flag is set
  if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
  {
    if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
    {
      return HAL_ERROR;
    }
    else
    {
      return HAL_TIMEOUT;
    }
  }

  // If Memory address size is 8Bit
  if(MemAddSize == I2C_MEMADD_SIZE_8BIT)
  {
    // Send Memory Address
    hi2c->Instance->TXDR = I2C_MEM_ADD_LSB(MemAddress);
  }
  // If Memory address size is 16Bit
  else
  {
    // Send MSB of Memory Address
    hi2c->Instance->TXDR = I2C_MEM_ADD_MSB(MemAddress);

    // Wait until TXIS flag is set
    if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    // Send LSB of Memory Address
    hi2c->Instance->TXDR = I2C_MEM_ADD_LSB(MemAddress);
  }

  // Wait until TC flag is set
  if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TC, RESET, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_TIMEOUT;
  }

  return HAL_OK;
}


HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  uint32_t tickstart = 0U;

  if(hi2c->State == HAL_I2C_STATE_READY)
  {
    // Process Locked
    __HAL_LOCK(hi2c);

    // Init tickstart for timeout management
    tickstart = HAL_GetTick();

    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    // Prepare transfer parameters
    hi2c->pBuffPtr  = pData;
    hi2c->XferCount = Size;
    hi2c->XferISR   = NULL;

    // Send Slave Address
    // Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART
    if(hi2c->XferCount > MAX_NBYTE_SIZE)
    {
      hi2c->XferSize = MAX_NBYTE_SIZE;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);
    }
    else
    {
      hi2c->XferSize = hi2c->XferCount;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);
    }

    while(hi2c->XferCount > 0U)
    {
      // Wait until TXIS flag is set
      if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }
      // Write data to TXDR
      hi2c->Instance->TXDR = (*hi2c->pBuffPtr++);
      hi2c->XferCount--;
      hi2c->XferSize--;

      if((hi2c->XferSize == 0U) && (hi2c->XferCount!=0U))
      {
        // Wait until TCR flag is set
        if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        if(hi2c->XferCount > MAX_NBYTE_SIZE)
        {
          hi2c->XferSize = MAX_NBYTE_SIZE;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
        }
        else
        {
          hi2c->XferSize = hi2c->XferCount;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        }
      }
    }

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is set
    if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
    {
      if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    // Process Unlocked
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}


  // * @brief  Receives in master mode an amount of data in blocking mode.
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  DevAddress Target device address: The device 7 bits address value
  // *         in datasheet must be shift at right before call interface
  // * @param  pData Pointer to data buffer
  // * @param  Size Amount of data to be sent
  // * @param  Timeout Timeout duration
  // * @retval HAL status

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  uint32_t tickstart = 0U;

  if(hi2c->State == HAL_I2C_STATE_READY)
  {
    // Process Locked
    __HAL_LOCK(hi2c);

    // Init tickstart for timeout management
    tickstart = HAL_GetTick();

    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

    hi2c->State = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    // Prepare transfer parameters
    hi2c->pBuffPtr  = pData;
    hi2c->XferCount = Size;
    hi2c->XferISR   = NULL;

    // Send Slave Address
    // Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART
    if(hi2c->XferCount > MAX_NBYTE_SIZE)
    {
      hi2c->XferSize = MAX_NBYTE_SIZE;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_READ);
    }
    else
    {
      hi2c->XferSize = hi2c->XferCount;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);
    }

    while(hi2c->XferCount > 0U)
    {
      // Wait until RXNE flag is set
      if(I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }

      // Read data from RXDR
      (*hi2c->pBuffPtr++) = hi2c->Instance->RXDR;
      hi2c->XferSize--;
      hi2c->XferCount--;

      if((hi2c->XferSize == 0U) && (hi2c->XferCount != 0U))
      {
        // Wait until TCR flag is set
        if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        if(hi2c->XferCount > MAX_NBYTE_SIZE)
        {
          hi2c->XferSize = MAX_NBYTE_SIZE;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
        }
        else
        {
          hi2c->XferSize = hi2c->XferCount;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        }
      }
    }

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is set
    if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
    {
      if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    // Process Unlocked
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}


HAL_StatusTypeDef HAL_I2C_Mem_Write(
    I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size,
    uint32_t Timeout) {
    uint32_t tickstart = 0U;


    if(hi2c->State == HAL_I2C_STATE_READY) {
        if((pData == NULL) || (Size == 0U))
            return  HAL_ERROR;


        // Process Locked
        __HAL_LOCK(hi2c);

        // Init tickstart for timeout management
        tickstart = HAL_GetTick();

        if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
            return HAL_TIMEOUT;


        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        // Prepare transfer parameters
        hi2c->pBuffPtr  = pData;
        hi2c->XferCount = Size;
        hi2c->XferISR   = NULL;

        // Send Slave Address and Memory Address
        if(I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK) {
            if(hi2c->ErrorCode == HAL_I2C_ERROR_AF) {
                // Process Unlocked
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            } else {
                // Process Unlocked
                __HAL_UNLOCK(hi2c);
                return HAL_TIMEOUT;
            }
        }

        // Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE
        if(hi2c->XferCount > MAX_NBYTE_SIZE) {
            hi2c->XferSize = MAX_NBYTE_SIZE;
            I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
        } else {
            hi2c->XferSize = hi2c->XferCount;
            I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        }

        do {
            // Wait until TXIS flag is set
            if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK) {
                if(hi2c->ErrorCode == HAL_I2C_ERROR_AF) {
                    return HAL_ERROR;
                } else {
                    return HAL_TIMEOUT;
                }
            }

            // Write data to TXDR
            hi2c->Instance->TXDR = (*hi2c->pBuffPtr++);
            hi2c->XferCount--;
            hi2c->XferSize--;

            if((hi2c->XferSize == 0U) && (hi2c->XferCount!=0U)) {
                // Wait until TCR flag is set
                if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK) {
                    return HAL_TIMEOUT;
                }

                if(hi2c->XferCount > MAX_NBYTE_SIZE) {
                    hi2c->XferSize = MAX_NBYTE_SIZE;
                    I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
                } else {
                    hi2c->XferSize = hi2c->XferCount;
                    I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
                }
            }

        } while(hi2c->XferCount > 0U);

        // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
        // Wait until STOPF flag is reset
        if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)  {
            if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
                return HAL_ERROR;
            else
                return HAL_TIMEOUT;
        }

        // Clear STOP Flag
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

        // Clear Configuration Register 2
        I2C_RESET_CR2(hi2c);

        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        // Process Unlocked
        __HAL_UNLOCK(hi2c);

        return HAL_OK;
    } else {
        return HAL_BUSY;
    }
}

  // * @brief  Read an amount of data in blocking mode from a specific memory address
  // * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  // *                the configuration information for the specified I2C.
  // * @param  DevAddress Target device address: The device 7 bits address value
  // *         in datasheet must be shift at right before call interface
  // * @param  MemAddress Internal memory address
  // * @param  MemAddSize Size of internal memory address
  // * @param  pData Pointer to data buffer
  // * @param  Size Amount of data to be sent
  // * @param  Timeout Timeout duration
  // * @retval HAL status

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c,
    uint16_t DevAddress,
    uint16_t MemAddress,
    uint16_t MemAddSize,
    uint8_t *pData,
    uint16_t Size,
    uint32_t Timeout) {
  uint32_t tickstart = 0U;


  if(hi2c->State == HAL_I2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    // Process Locked
    __HAL_LOCK(hi2c);

    // Init tickstart for timeout management
    tickstart = HAL_GetTick();

    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    // Prepare transfer parameters
    hi2c->pBuffPtr  = pData;
    hi2c->XferCount = Size;
    hi2c->XferISR   = NULL;

    // Send Slave Address and Memory Address
    if(I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)
    {
      if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        // Process Unlocked
        __HAL_UNLOCK(hi2c);
        return HAL_ERROR;
      }
      else
      {
        // Process Unlocked
        __HAL_UNLOCK(hi2c);
        return HAL_TIMEOUT;
      }
    }

    // Send Slave Address
    // Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE and generate RESTART
    if(hi2c->XferCount > MAX_NBYTE_SIZE)
    {
      hi2c->XferSize = MAX_NBYTE_SIZE;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_READ);
    }
    else
    {
      hi2c->XferSize = hi2c->XferCount;
      I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);
    }

    do
    {
      // Wait until RXNE flag is set
      if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET, Timeout, tickstart) != HAL_OK)
      {
        return HAL_TIMEOUT;
      }

      // Read data from RXDR
      (*hi2c->pBuffPtr++) = hi2c->Instance->RXDR;
      hi2c->XferSize--;
      hi2c->XferCount--;

      if((hi2c->XferSize == 0U) && (hi2c->XferCount != 0U))
      {
        // Wait until TCR flag is set
        if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        if(hi2c->XferCount > MAX_NBYTE_SIZE)
        {
          hi2c->XferSize = MAX_NBYTE_SIZE;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
        }
        else
        {
          hi2c->XferSize = hi2c->XferCount;
          I2C_TransferConfig(hi2c, DevAddress, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        }
      }
    }while(hi2c->XferCount > 0U);

    // No need to Check TC flag, with AUTOEND mode the stop is automatically generated
    // Wait until STOPF flag is reset
    if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
    {
      if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    // Clear STOP Flag
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    // Clear Configuration Register 2
    I2C_RESET_CR2(hi2c);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode  = HAL_I2C_MODE_NONE;

    // Process Unlocked
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

