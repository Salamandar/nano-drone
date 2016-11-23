#include "mpu.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

#define I2C1_SDA_PORT   GPIOB
#define I2C1_SDA_PIN    GPIO7

#define I2C1_SCL_PORT   GPIOB
#define I2C1_SCL_PIN    GPIO6

void init_mpu() {
    // rcc_periph_clock_enable(RCC_I2C1);
    // //rcc_set_i2c_clock_hsi(I2C1);

    // i2c_reset(I2C1);
    // // Setup GPIO pin GPIO_USART2_TX/GPIO9 on GPIO port A for transmit.
    // gpio_mode_setup(
    //     GPIOB,
    //     GPIO_MODE_AF, GPIO_PUPD_NONE,
    //     GPIO6 | GPIO7);
    // gpio_set_af(
    //     GPIOB,
    //     GPIO_AF4,
    //     GPIO6 | GPIO7);

    // i2c_peripheral_disable  (I2C1);
    // //i2c_set_digital_filter  (I2C1, I2C_CR1_DNF_DISABLED);
    // // Configure PRESC[3:0] SDADEL[3:0] SCLDEL[3:0] SCLH[7:0] SCLL[7:0]
    // // in TIMINGR
    // i2c_100khz_i2cclk8mhz   (I2C1);
    // // configure No-Stretch CR1 (only relevant in slave mode)
    // i2c_enable_stretching   (I2C1);
    // // addressing mode
    // i2c_set_7bit_addr_mode  (I2C1);
    // i2c_peripheral_enable   (I2C1);

}

// #define I2C_ACC_ADDR 0x19
// #define I2C_MAG_ADDR 0x1E
// #define ACC_STATUS 0x27
// #define ACC_CTRL_REG1_A 0x20
// #define ACC_CTRL_REG1_A_ODR_SHIFT 4
// #define ACC_CTRL_REG1_A_ODR_MASK 0xF
// #define ACC_CTRL_REG1_A_XEN (1 << 0)
// #define ACC_CTRL_REG4_A 0x23

// #define ACC_OUT_X_L_A 0x28
// #define ACC_OUT_X_H_A 0x29

//     write_i2c(I2C1, I2C_ACC_ADDR, ACC_CTRL_REG1_A, 1, data);
//     data[0]=0x08;
//     write_i2c(I2C1, I2C_ACC_ADDR, ACC_CTRL_REG4_A, 1, data);
//     uint16_t acc_x;

//     while (1) {

//         read_i2c(I2C1, I2C_ACC_ADDR, ACC_STATUS, 1, data);
//         /*my_usart_print_int(USART2, data[0]);*/
//         read_i2c(I2C1, I2C_ACC_ADDR, ACC_OUT_X_L_A, 1, data);
//         acc_x=data[0];
//         read_i2c(I2C1, I2C_ACC_ADDR, ACC_OUT_X_H_A, 1, data);
//         acc_x|=(data[0] << 8);
//         my_usart_print_int(USART2, (int16_t) acc_x);
//         //int i;
//         //for (i = 0; i < 800000; i++)    /* Wait a bit. */
//         //  __asm__("nop");
//     }

//     return 0;
// }
