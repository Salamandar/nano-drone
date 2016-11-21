#include "hardware.h"
#include "leds.h"
#include "motors.h"
#include "mpu.h"
#include "radio.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void init_hardware() {
    // rcc_clock_setup_in_hsi_out_8mhz();
    // rcc_clock_setup_in_hsi_out_16mhz();
    // rcc_clock_setup_in_hsi_out_24mhz();
    // rcc_clock_setup_in_hsi_out_32mhz();
    // rcc_clock_setup_in_hsi_out_40mhz();
    rcc_clock_setup_in_hsi_out_48mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    init_leds();
    init_motors();
    init_mpu();
    init_radio();

}


void delay_nop(unsigned int count) {
    for (int i = 0; i < count; ++i)
        __asm__("nop");
}
