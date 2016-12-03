#pragma once

#include <stdint.h>

// The order should not change, new values should only be added at the end
typedef struct Flash_Values {
    uint32_t trim_yaw;
    uint32_t trim_pitch;
    uint32_t trim_roll;

    uint32_t speed_mode;

    uint32_t last_led_set;
    uint32_t rf_address;
    uint32_t rf_channel;

} Flash_Values;


void eeprom_test();

// Read from Flash
void     read_flash_values();
// Get the R/W structure
Flash_Values *flash_values();
// Save to Flash
void     save_flash_values();
