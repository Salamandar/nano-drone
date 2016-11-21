#pragma once

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ   (48000000)

// Prescale 48MHz system clock by 48 = 1MHz.
#define PWM_PRESCALE        (48)

// We need a 50Hz period (1000 / 20ms = 50), thus devide 100000 by 50 = 20000 (us).
#define PWM_PERIOD          (2000)


void init_hardware();
void delay_nop(unsigned int count);

