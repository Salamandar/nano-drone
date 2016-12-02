#include "hardware.h"
#include "leds.h"
#include "motors.h"
#include "mpu.h"
#include "radio.h"
#include "video.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

void init_hardware() {
    rcc_clock_setup_in_hsi_out_48mhz();

    // 48MHz => 48000000 counts per second.
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // Interrupts each millisec
    systick_set_reload(48000 - 1);
    // clear counter so it starts right away
    systick_clear();
    systick_counter_enable();
    systick_interrupt_enable();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    init_leds();
    init_motors();
    init_mpu();
    init_radio();
    init_video_pin();

}


volatile int systick_count = 0;

void sys_tick_handler() {
    systick_count++;
    // xPortSysTickHandler();
}


// Should not be used with FreeRTOS !!
void delay_nop(unsigned int count) {
    for (long int i = 0; i < count; ++i)
        __asm__("nop");
}

void delay_ms(unsigned int ms) {
    int count_max = systick_count + ms;
    while(systick_count < count_max) {}
}



// #include "stm32f0xx_hal.h"

// void SystemClock_Config(void);
// void Error_Handler(void);
// static void MX_GPIO_Init(void);
// static void MX_I2C1_Init(void);

// /* Private variables ---------------------------------------------------------*/
// I2C_HandleTypeDef hi2c1;

// void init_hardware() {
//     HAL_Init();

//     /* Configure the system clock */
//     SystemClock_Config();

//     MX_GPIO_Init();
//     // MX_I2C1_Init();



// }


// void SystemClock_Config(void)
// {

//   RCC_OscInitTypeDef RCC_OscInitStruct;
//   RCC_ClkInitTypeDef RCC_ClkInitStruct;
//   RCC_PeriphCLKInitTypeDef PeriphClkInit;

//     /**Initializes the CPU, AHB and APB busses clocks
//     */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSICalibrationValue = 16;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//     /**Initializes the CPU, AHB and APB busses clocks
//     */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
//   PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
//   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//   {
//     Error_Handler();
//   }

//     /**Configure the Systick interrupt time
//     */
//   HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

//     /**Configure the Systick
//     */
//   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

//   /* SysTick_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
// }

// /* I2C1 init function */
// static void MX_I2C1_Init(void)
// {

//   hi2c1.Instance = I2C1;
//   hi2c1.Init.Timing = 0x2000090E;
//   hi2c1.Init.OwnAddress1 = 0;
//   hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c1.Init.OwnAddress2 = 0;
//   hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//   hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//   {
//     Error_Handler();
//   }

//     /**Configure Analogue filter
//     */
//   if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//   {
//     Error_Handler();
//   }

// }

// /** Pinout Configuration
// */
// static void MX_GPIO_Init(void)
// {

//     /* GPIO Ports Clock Enable */
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     GPIO_InitTypeDef GPIO_InitStruct;
//     GPIO_InitStruct.Pin = GPIO_PIN_4;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
// }

// /* USER CODE BEGIN 4 */

// /* USER CODE END 4 */

// /**
//   * @brief  This function is executed in case of error occurrence.
//   * @param  None
//   * @retval None
//   */
// void Error_Handler(void)
// {
//   /* USER CODE BEGIN Error_Handler */
//   /* User can add his own implementation to report the HAL error return state */
//   while(1)
//   {
//   }
//   /* USER CODE END Error_Handler */
// }

// void HAL_MspInit(void)
// {
//   /* USER CODE BEGIN MspInit 0 */

//   /* USER CODE END MspInit 0 */

//   __HAL_RCC_SYSCFG_CLK_ENABLE();

//   /* System interrupt init*/
//   /* SVC_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
//   /* PendSV_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
//   /* SysTick_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

//   /* USER CODE BEGIN MspInit 1 */

//   /* USER CODE END MspInit 1 */
// }

// void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
// {

//   GPIO_InitTypeDef GPIO_InitStruct;
//   if(hi2c->Instance==I2C1)
//   {
//   /* USER CODE BEGIN I2C1_MspInit 0 */

//   /* USER CODE END I2C1_MspInit 0 */

//     /**I2C1 GPIO Configuration
//     PB6     ------> I2C1_SCL
//     PB7     ------> I2C1_SDA
//     */
//     GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     /* Peripheral clock enable */
//     __HAL_RCC_I2C1_CLK_ENABLE();
//   /* USER CODE BEGIN I2C1_MspInit 1 */

//   /* USER CODE END I2C1_MspInit 1 */
//   }

// }


// #include "stm32f0xx.h"
// #include "stm32f0xx_hal_i2c.h"


// #define MPU6050_RA_WHO_AM_I         0x75
// #define Slave_address   0x68
// #define mpu6050_FLAG_TIMEOUT         ((uint32_t)0x1000)
// #define mpu6050_LONG_TIMEOUT         ((uint32_t)(500 * mpu6050_FLAG_TIMEOUT))

// uint8_t ReadReg(uint8_t RegName)
// {
//   uint8_t mpu6050_BufferRX[2] ={0,0};
//   uint8_t tmp = 0;
//   uint32_t DataNum = 0;
//   int mpu6050_Timeout = 0;

//   /* Test on BUSY Flag */
//   mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
//   while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
//   {
//     if((mpu6050_Timeout--) == 0) USART_print(" error1 ");
//   }


//   /* Wait until TXIS flag is set */
//   mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
//   while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
//   {
//     if((mpu6050_Timeout--) == 0) USART_print(" error2 ");
//   }

//   /* Send Register address */
//   I2C_SendData(I2C1, (uint8_t)RegName);

//   /* Wait until TC flag is set */
//   mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
//   while(I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET)
//   {
//     if((mpu6050_Timeout--) == 0) USART_print(" error3 ");
//   }

//   /* Reset local variable */
//   DataNum = 0;

//   /* Wait until all data are received */
//   while (DataNum != 1)
//   {
//     /* Wait until RXNE flag is set */
//       mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
//     while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET)
//     {
//       if((mpu6050_Timeout--) == 0) USART_print(" error4 ");
//     }

//     /* Read data from RXDR */
//     mpu6050_BufferRX[DataNum]= I2C_ReceiveData(I2C1);

//     /* Update number of received data */
//     DataNum++;
//   }

//   /* Wait until STOPF flag is set */
//   mpu6050_Timeout = mpu6050_LONG_TIMEOUT;
//   while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
//   {
//     if((mpu6050_Timeout--) == 0) USART_print(" error5 ");
//   }

//   /* Clear STOPF flag */
//   I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


//   // !< Store LM75_I2C received data
//   //tmp = (uint16_t)(LM75_BufferRX[0] << 8);
//   //tmp |= LM75_BufferRX[0];
//   tmp = mpu6050_BufferRX[0];
//   // return a Reg value
//   return (uint8_t)tmp;



// }

// void hw_main() {
//     while(1) {

//         ReadReg(MPU6050_RA_WHO_AM_I);

//         for(int i=0;i<10000000;i++);

//         /*

//         int i=0;
//         if(USART_ReceiveData(USART1)=='x'){

//         GPIO_SetBits(GPIOC, GPIO_Pin_8);
//         }
//         for(i=0;i<1000000;i++);

//         GPIO_ResetBits(GPIOC, GPIO_Pin_8);

//         for(i=0;i<1000000;i++);

//         */
//     }
// }
