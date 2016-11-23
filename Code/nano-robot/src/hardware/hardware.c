#include "hardware.h"
#include "leds.h"
#include "motors.h"
#include "mpu.h"
#include "radio.h"
#include "video.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

void init_hardware() {
    rcc_clock_setup_in_hsi_out_48mhz();

    // 48MHz => 48000000 counts per second.
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // Interrupts each millisec
    systick_set_reload(48000 - 1);
    // clear counter so it starts right away
    systick_clear();
    systick_counter_enable();
    systick_interrupt_enable();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    init_leds();
    init_motors();
    init_mpu();
    init_radio();
    init_video_pin();

}


// Should not be used with FreeRTOS !!
void delay_nop(unsigned int count) {
    for (int i = 0; i < count; ++i)
        __asm__("nop");
}
void delay_nop_ms(unsigned int ms) {
    for (int i = 0; i < ms*RCC_CLOCK_FREQ_HZ; ++i)
        __asm__("nop");
}
