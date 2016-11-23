#include "leds.h"

#include <libopencm3/stm32/gpio.h>


enum LedsPort {
    LedsBleuesPort = GPIOA,
    LedsRougesPort = GPIOB
};
enum LedsPin {
    LedsBleuesPin = GPIO4,
    LedsRougesPin = GPIO1
};

void init_leds() {
    gpio_mode_setup(LedsBleuesPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LedsBleuesPin);
    gpio_mode_setup(LedsRougesPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LedsRougesPin);
}

void toggle_leds_bleues() { gpio_toggle(LedsBleuesPort, LedsBleuesPin); }
void    set_leds_bleues() { gpio_set   (LedsBleuesPort, LedsBleuesPin); }
void  clear_leds_bleues() { gpio_clear (LedsBleuesPort, LedsBleuesPin); }

void toggle_leds_rouges() {
    __asm("BKPT");
    gpio_toggle(LedsRougesPort, LedsRougesPin);
}
void    set_leds_rouges() { gpio_set   (LedsRougesPort, LedsRougesPin); }
void  clear_leds_rouges() { gpio_clear (LedsRougesPort, LedsRougesPin); }
