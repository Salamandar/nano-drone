#pragma once

#include <libopencm3/stm32/gpio.h>

enum LedsPort {
    LedBleuesPort = GPIOA,
    LedRougesPort = GPIOB
};
enum LedsPin {
    LedBleuesPin = GPIO4,
    LedRougesPin = GPIO1
};

void init_hardware();
void delay_nop(unsigned int count);
