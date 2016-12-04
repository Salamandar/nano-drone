#include "hardware.h"
#include "mpu.h"
#include "mpu_MPU-6052C.h"

#include "HAL/main_i2c.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

#include <machine/endian.h>

const uint16_t I2C_MPU_ADDR = 0xd0;
uint8_t MPU_id;

Coords accel;
Coords accel_cal;

Coords gyro;
Coords gyro_cal;


inline
void MPU_write_register(uint16_t MemAddress, uint8_t data) {
    i2c_HAL_write_register(I2C_MPU_ADDR, MemAddress, &data, 1);
}

void init_mpu() {
    // TODO interrupt on gpio useful ?
    gpio_mode_setup(GPIOA,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        GPIO0);

    // TODO clean HAL and debug libopencm3
    gpio_mode_setup(GPIOB,
        GPIO_MODE_AF,
        GPIO_PUPD_PULLUP,
        GPIO6 | GPIO7);

    gpio_set_output_options(GPIOB,
        GPIO_OTYPE_OD,
        GPIO_OSPEED_HIGH,
        GPIO6 | GPIO7);
    gpio_set_af(GPIOB,
        GPIO_AF1,
        GPIO6 | GPIO7);

    rcc_periph_clock_enable(RCC_I2C1);
    I2C_TIMINGR(I2C1) = 0x0000020B & 0xF0FFFFFF;
    i2c_peripheral_disable  (I2C1);
    i2c_set_7bit_addr_mode  (I2C1);
    i2c_enable_stretching   (I2C1);
    i2c_enable_analog_filter(I2C1);
    i2c_peripheral_enable   (I2C1);

    init_i2c_HAL();
    delay_ms(1);

    // Gyro soft-reset
    MPU_write_register(PWR_MGMT_1,  0b10000000);
    delay_ms(40);
    // …Then back to life !
    MPU_write_register(PWR_MGMT_1,  0b00000000);

    // Gyro FS (FullScale) range 3 (±2000°/s)
    MPU_write_register(GYRO_CONFIG, 0b00011000);
    // Gyro+Accelero DLPF (Digital Low Pass Filter)
    MPU_write_register(CONFIG,      GYRO_LOW_PASS_42Hz);

    // Get the MPU identifier for
    i2c_HAL_read(WHO_AM_I, &MPU_id, 1);
}

// TODO this should be in a regular interrupt !
void mpu_get_inertial_values() {
    uint16_t data[3];

    // We can get multiple registers at once
    i2c_HAL_read_register(I2C_MPU_ADDR, ACCEL_XOUT_H, (uint8_t*)data, 6);
    accel.x = __bswap16(data[0]);
    accel.y = __bswap16(data[1]);
    accel.z = __bswap16(data[2]);

    // We can get multiple registers at once
    i2c_HAL_read_register(I2C_MPU_ADDR,  GYRO_XOUT_H, (uint8_t*)data, 6);
    gyro.x = __bswap16(data[0]);
    gyro.y = __bswap16(data[1]);
    gyro.z = __bswap16(data[2]);

    // BSwap is needed for big/little endian conversion
    // (The bus is big-endian, the Cortex-MO is little-endian)
}


// This function should be called until it returns true.
bool mpu_calibrate() {



}

