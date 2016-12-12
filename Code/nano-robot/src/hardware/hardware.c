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
    // Interrupts each millisec tenth
    systick_set_reload(SYSTICK_PERIOD - 1);
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

// Systick management
volatile int systick_count = 0;

void sys_tick_handler() {
    systick_count++;
    // xPortSysTickHandler();
}

int get_systick() {
    return systick_count;
}


// Should not be used with FreeRTOS !!
void delay_nop(unsigned int count) {
    for (long int i = 0; i < count; ++i)
        __asm__("nop");
}

void delay_ms(unsigned int ms) {
    int count_max = systick_count + ms * SYSTICK_FREQ_HZ / 1000;
    while(systick_count < count_max) {}
}
