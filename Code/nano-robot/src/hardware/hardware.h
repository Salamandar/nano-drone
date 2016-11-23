#pragma once

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (48000000)

void init_hardware();
void delay_nop(unsigned int count);
