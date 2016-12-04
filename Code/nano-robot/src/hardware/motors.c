#include "motors.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

void init_motors(void) {
    // Enable timer clock
    rcc_peripheral_enable_clock(&RCC_APB2ENR,
                                RCC_APB2ENR_TIM1EN);

    // Reset TIM1 peripheral
    timer_reset   (TIM1);
    timer_set_mode(TIM1,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler         (TIM1, PWM_PRESCALE);
    timer_set_repetition_counter(TIM1, 0);
    timer_enable_preload        (TIM1);
    timer_continuous_mode       (TIM1);
    timer_set_period            (TIM1, PWM_PERIOD);

    // start timer1
    timer_enable_break_main_output(TIM1);
    timer_enable_counter        (TIM1);


    // Enable GPIO clock
    rcc_peripheral_enable_clock(&RCC_AHBENR, RCC_AHBENR_GPIOAEN);

    // Set timer channel to output
    gpio_set_output_options(
                GPIOA,
                GPIO_OTYPE_PP,
                GPIO_OSPEED_HIGH,
                GPIO8 | GPIO9 | GPIO10 | GPIO11);
    gpio_mode_setup(
                GPIOA,
                GPIO_MODE_AF,
                GPIO_PUPD_NONE,
                GPIO8 | GPIO9 | GPIO10 | GPIO11);
    gpio_set_af(GPIOA,
                GPIO_AF2,
                GPIO8 | GPIO9 | GPIO10 | GPIO11);

    timer_disable_oc_output (TIM1, TIM_OC1);
    timer_set_oc_mode       (TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_value      (TIM1, TIM_OC1, 0);
    timer_enable_oc_output  (TIM1, TIM_OC1);

    timer_disable_oc_output (TIM1, TIM_OC2);
    timer_set_oc_mode       (TIM1, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_value      (TIM1, TIM_OC2, 0);
    timer_enable_oc_output  (TIM1, TIM_OC2);

    timer_disable_oc_output (TIM1, TIM_OC3);
    timer_set_oc_mode       (TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_value      (TIM1, TIM_OC3, 0);
    timer_enable_oc_output  (TIM1, TIM_OC3);

    timer_disable_oc_output (TIM1, TIM_OC4);
    timer_set_oc_mode       (TIM1, TIM_OC4, TIM_OCM_PWM1);
    timer_set_oc_value      (TIM1, TIM_OC4, 0);
    timer_enable_oc_output  (TIM1, TIM_OC4);
}

// TODO arranger switch
void motor_set_speed(Motor motor, uint32_t speed) {
    speed = speed <=        0 ?         0
          : speed > motor_max ? motor_max
          : speed < motor_min ? motor_min
          : speed ;

    timer_set_oc_value(TIM1, motor, speed);
}
