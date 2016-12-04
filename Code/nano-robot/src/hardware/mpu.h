#pragma once

#include <stdint.h>


typedef struct __attribute__((packed)) coords {
    int16_t x;
    int16_t y;
    int16_t z;
} Coords;

// Global variables ? Meeeeeh.
// MPU identifier. Well_known values : 0x78, 0x68, 0x7D
extern uint8_t MPU_id;

extern Coords accel;
extern Coords accel_cal;

extern Coords gyro;
extern Coords gyro_cal;

void init_mpu();
void mpu_get_inertial_values();
