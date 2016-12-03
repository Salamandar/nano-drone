#include "hardware.h"
#include "mpu.h"
#include "mpu_MPU-6052C.h"

#include "leds.h"
#include "motors.h"

#include "HAL/main_i2c.h"

#include <libopencm3/stm32/gpio.h>

#define I2C_MPU_ADDR    0xd0

uint8_t to_send[1];
uint8_t receive[1];

inline
void MPU_write_register(uint16_t MemAddress, uint8_t data) {
    to_send[0] = data;
    i2c_HAL_write_register(I2C_MPU_ADDR, MemAddress, to_send, 1);
}

void init_mpu() {
    // TODO interrupt on gpio useful ?
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);

    // TODO clean HAL and debug libopencm3
    init_i2c_HAL();
    delay_ms(1);

    // Gyro soft-reset
    MPU_write_register(107,  128);
    delay_ms(40);
    // …Then back to life !
    MPU_write_register(107,  0);

    // Gyro FS (FullScale) range 3 (±2000°/s)
    MPU_write_register(27, 24);
    // Gyro+Accelero DLPF (Digital Low Pass Filter)
    MPU_write_register(26,      GYRO_LOW_PASS_42Hz);
}

void mpu_who_are_you() {
    i2c_HAL_read(WHO_AM_I, receive, 1);
    int id = receive[0];
    // Well_known values : 0x78, 0x68, 0x7D
}


int16_t accel_x;
int16_t accel_y;
int16_t accel_z;

int16_t accel_cal_x;
int16_t accel_cal_y;
int16_t accel_cal_z;

int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;

int16_t gyro_cal_x;
int16_t gyro_cal_y;
int16_t gyro_cal_z;


#define COS45 1.41421356237/2


void mpu_get_inertial_values() {
    uint8_t data[6];

    // We can get multiple registers at once
    i2c_HAL_read_register(I2C_MPU_ADDR, ACCEL_XOUT_H, data, 6);

    accel_x = (( ((int16_t) data[0]) << 8) | data[1]) - accel_cal_x;
    accel_y = (( ((int16_t) data[2]) << 8) | data[3]) - accel_cal_y;
    accel_z = (( ((int16_t) data[4]) << 8) | data[5]) - accel_cal_z;


    // We can get multiple registers at once
    i2c_HAL_read_register(I2C_MPU_ADDR,  GYRO_XOUT_H, data, 6);

    gyro_x  = (( ((int16_t) data[0]) << 8) | data[1]) - gyro_cal_x;
    gyro_y  = (( ((int16_t) data[2]) << 8) | data[3]) - gyro_cal_y;
    gyro_z  = (( ((int16_t) data[4]) << 8) | data[5]) - gyro_cal_z;


    int16_t axis02 = COS45*(accel_x+accel_y);
    int16_t axis13 = COS45*(accel_x-accel_y);

    if (axis02 > 0) {
        motor_set_speed(2,  axis02/10);
        motor_set_speed(0, 0);
    }
    else {
        motor_set_speed(0, -axis02/10);
        motor_set_speed(2, 0);
    }

    if (axis13 > 0) {
        motor_set_speed(3,  axis13/10);
        motor_set_speed(1, 0);
    }
    else {
        motor_set_speed(1, -axis13/10);
        motor_set_speed(3, 0);
    }
}


// This function should be called until it returns true.
bool mpu_calibrate() {



}
