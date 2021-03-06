#pragma once

#define BP __asm__("BKPT")

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (48000000)
#define SYSTICK_FREQ_HZ     (1000)
#define SYSTICK_PERIOD      (48000000/SYSTICK_FREQ_HZ)

void init_hardware();

int get_systick();
void delay_nop(unsigned int count);
void delay_ms(unsigned int ms);

