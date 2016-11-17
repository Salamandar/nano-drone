#include "hardware.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

// Timer Clock Frequency [Hz]
#define TIM_CLOCK_FREQ_HZ           48000000

void init_motor_pwm() {
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_TIM1EN);
    timer_reset(TIM1);
    timer_set_mode(TIM1,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, 48);
    timer_set_repetition_counter(TIM1, 0);
    timer_enable_preload(TIM1);
    timer_continuous_mode(TIM1);
    timer_set_period(TIM1, 2000);


//TIM2, TIM_OC2, &RCC_APB2ENR, RCC_APB2ENR_IOPAEN, GPIOA, GPIO_TIM2_CH2

//GPIO_MODE_AF
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
        GPIO8 | GPIO9 | GPIO10 | GPIO11);

    timer_disable_oc_output(TIM1, TIM_OC2);
    timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_value(TIM1, TIM_OC2, 0);
    timer_enable_oc_output(TIM1, TIM_OC2);

    timer_enable_counter(TIM1);
    timer_set_oc_value(TIM1, TIM_OC2, 1500);
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
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
        GPIO8 | GPIO9 | GPIO10 | GPIO11);
    // gpio_set_af(GPIOA, GPIO_AF2,
    //     GPIO8 | GPIO9 | GPIO10 | GPIO11);

//    init_motor_pwm();



    // pwm_set_duty(TIM14, TIM_OC1, 0.5);
    // pwm_set_duty(TIM14, TIM_OC2, 0.5);
    // pwm_set_duty(TIM14, TIM_OC3, 0.5);
    // pwm_set_duty(TIM14, TIM_OC4, 0.5);

}


void delay_nop(unsigned int count) {
    for (int i = 0; i < count; ++i)
        __asm__("nop");
}
