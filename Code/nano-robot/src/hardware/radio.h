#pragma once

#include <stdbool.h>
#include <stdint.h>

void init_radio();

// Useful when the RC channel changes
void nrf_setChann(int chan);

// Get the data (if received) in the data_buffer
bool nrf_receive_data(int size_max, uint8_t *data_buffer);
