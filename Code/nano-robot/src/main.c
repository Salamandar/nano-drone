#include "hardware.h"

#include <libopencm3/stm32/gpio.h>


int main() {
    init_hardware();

    //gpio_set(LedRougesPort, LedRougesPin);

    while(1) {
        delay_nop(400000);
        gpio_toggle(LedBleuesPort, LedBleuesPin);
        gpio_toggle(LedRougesPort, LedRougesPin);

        // gpio_toggle(GPIOA, GPIO8 | GPIO9 | GPIO10 | GPIO11);

    }

}

