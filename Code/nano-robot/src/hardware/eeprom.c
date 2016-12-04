#include "eeprom.h"

#include "hardware.h"
#include "leds.h"

#include <libopencm3/stm32/flash.h>

// Stm32f031k4 specific !
#define BASE_ADDRESS                (0x08003000)
#define FLASH_PAGE_NUM_MAX          (16)
#define FLASH_PAGE_SIZE             (0x400)
#define FLASH_WRONG_DATA_WRITTEN    (0x80)


// Currently only works if all the data is in the same page
uint32_t flash_write_data(uint32_t start_address, uint32_t *input_data, uint16_t num_elements) {
    // Check if addresses are in the flash
    if (start_address                   <   FLASH_BASE
    && (start_address + num_elements/4) >= (FLASH_BASE + FLASH_PAGE_SIZE * FLASH_PAGE_NUM_MAX) )
        return 1;

    // calculate the page address
    uint32_t page_address = start_address - (start_address % FLASH_PAGE_SIZE);

    // Status flags
    volatile uint32_t flash_status = 0;

    // Ask the STM32 to unlock the flash for write
    flash_unlock();

    // Erasing the page is needed
    flash_erase_page(page_address);

    if((flash_status = flash_get_status_flags()) != FLASH_SR_EOP)
        return flash_status;

    // programming flash memory
    for (int i = 0; i < num_elements; ++i) {
        flash_program_word(start_address + i*4, *((uint32_t*)input_data + i));

        if((flash_status = flash_get_status_flags()) != FLASH_SR_EOP)
            return flash_status;

#       ifdef DEBUG_EEPROM
        // verify if correct data is programmed
        if (*((uint32_t*)start_address + i)
         != *((uint32_t*)input_data    + i) )
            return FLASH_WRONG_DATA_WRITTEN;
#       endif
    }
    return 0;
}

void flash_read_data(uint32_t start_address, uint32_t *output_data, uint16_t num_elements) {
    for (uint16_t i = 0; i < num_elements; ++i)
        *((uint32_t*)output_data + i) = *((uint32_t*)start_address + i);
}


// Read and write a structure for a simpler value management

Flash_Values the_flash_values;

void read_flash_values() {
    flash_read_data (BASE_ADDRESS, (uint32_t*) &the_flash_values, sizeof(the_flash_values) / 4);
}

Flash_Values *flash_values() {
    return &the_flash_values;
}

void save_flash_values() {
    flash_write_data(BASE_ADDRESS, (uint32_t*) &the_flash_values, sizeof(the_flash_values) / 4);
}

// A simple toggle leds at boot !
void eeprom_test() {
    read_flash_values();

    switch(the_flash_values.last_led_set) {
    case 0:
        set_leds_rouges();
        break;
    case 1:
        set_leds_bleues();
        break;
    case 2:
        set_leds_rouges();
        set_leds_bleues();
        break;
    }
    the_flash_values.last_led_set = (the_flash_values.last_led_set + 1) % 3;

    save_flash_values();
}
