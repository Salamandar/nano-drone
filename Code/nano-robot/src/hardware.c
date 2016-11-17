#include "hardware.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// RCC Clock Frequency [Hz]
#define RCC_CLOCK_FREQ_HZ           48000000

// Prescale 48MHz system clock by 48 = 1MHz.
#define PWM_PRESCALE    (48)

// We need a 50Hz period (1000 / 20ms = 50), thus devide 100000 by 50 = 20000 (us).
#define PWM_PERIOD      (20000)


void pwm_init_timer(volatile uint32_t *reg,
                             uint32_t en,
                             uint32_t timer_peripheral,
                             uint32_t prescaler,
                             uint32_t period) {
    // Enable timer clock
    rcc_peripheral_enable_clock(reg, en);

    // Reset TIM1 peripheral
    timer_reset(timer_peripheral);

    // Set the timers global mode to:
    // - use no divider
    // - alignment edge
    // - count direction up

    timer_set_mode(timer_peripheral,
                    TIM_CR1_CKD_CK_INT,
                    TIM_CR1_CMS_EDGE,
                    TIM_CR1_DIR_UP);

    timer_set_prescaler(timer_peripheral, prescaler);
    //timer_set_repetition_counter(timer_peripheral, 0);
    timer_enable_preload(timer_peripheral);
    timer_continuous_mode(timer_peripheral);
    timer_set_period(timer_peripheral, period);
}

void pwm_init_output_channel(uint32_t timer_peripheral,
                             enum tim_oc_id oc_id,
                    volatile uint32_t *gpio_reg,
                             uint32_t  gpio_en,
                             uint32_t  gpio_port,
                             uint16_t  gpio_pin,
                             uint8_t altfn) {
    // Enable GPIO clock
    rcc_peripheral_enable_clock(gpio_reg, gpio_en);

    // Set timer channel to output
    gpio_set_output_options(gpio_port, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, gpio_pin);
    gpio_mode_setup(gpio_port, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);
    gpio_set_af(gpio_port, gpio_pin, altfn);

    timer_disable_oc_output(timer_peripheral, oc_id);
    timer_set_oc_mode(timer_peripheral, oc_id, TIM_OCM_PWM1);
    timer_set_oc_value(timer_peripheral, oc_id, 0);
    timer_enable_oc_output(timer_peripheral, oc_id);
}

void pwm_set_pulse_width(uint32_t timer_peripheral, enum tim_oc_id oc_id, uint32_t pulse_width) {
    timer_set_oc_value(timer_peripheral, oc_id, pulse_width);
}

void pwm_start_timer(uint32_t timer_peripheral) {
    timer_enable_counter(timer_peripheral);
}

void moteurs_init(void) {
    // init timer2 with a period of 20ms
    pwm_init_timer(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN, TIM1, PWM_PRESCALE, PWM_PERIOD);

    // init output of channel2 of timer2
    // pwm_init_output_channel(TIM1, TIM_OC1, &RCC_AHBENR, RCC_AHBENR_GPIOAEN, GPIOA, GPIO8,  GPIO_AF2);
    pwm_init_output_channel(TIM1, TIM_OC2, &RCC_AHBENR, RCC_AHBENR_GPIOAEN, GPIOA, GPIO9,  GPIO_AF2);
    pwm_init_output_channel(TIM1, TIM_OC3, &RCC_AHBENR, RCC_AHBENR_GPIOAEN, GPIOA, GPIO10, GPIO_AF2);
    pwm_init_output_channel(TIM1, TIM_OC4, &RCC_AHBENR, RCC_AHBENR_GPIOAEN, GPIOA, GPIO11, GPIO_AF2);


    // pwm_set_pulse_width(TIM1, TIM_OC1, PWM_PERIOD);
    pwm_set_pulse_width(TIM1, TIM_OC2, PWM_PERIOD);
    pwm_set_pulse_width(TIM1, TIM_OC3, PWM_PERIOD);
    pwm_set_pulse_width(TIM1, TIM_OC4, PWM_PERIOD);

    // start timer1
    pwm_start_timer(TIM1);
}

void init_hardware() {
    // rcc_clock_setup_in_hsi_out_8mhz();
    // rcc_clock_setup_in_hsi_out_16mhz();
    // rcc_clock_setup_in_hsi_out_24mhz();
    // rcc_clock_setup_in_hsi_out_32mhz();
    // rcc_clock_setup_in_hsi_out_40mhz();
    rcc_clock_setup_in_hsi_out_48mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    // Leds
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

    // Moteurs
    // gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
    //     GPIO8 | GPIO9 | GPIO10 | GPIO11);
    moteurs_init();

}


void delay_nop(unsigned int count) {
    for (int i = 0; i < count; ++i)
        __asm__("nop");
}



void servo_set_position(enum tim_oc_id ch, uint32_t pos_us)
{
     pwm_set_pulse_width(TIM2, ch, pos_us);
}
