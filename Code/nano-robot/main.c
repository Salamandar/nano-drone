#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>



int main_blink(void) {
    rcc_clock_setup_in_hsi_out_48mhz();;

    rcc_periph_clock_enable(RCC_GPIOA);
    //rcc_periph_clock_enable(RCC_GPIOG);


    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO4); // Bouton User
    gpio_set  (GPIOA,  GPIO4);
    // gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
    //     GPIO13 | GPIO14);

    while(1) {
        // gpio_toggle(GPIOG, GPIO13);
    }
    // new_xy_absolu(300,1600);
    // new_xy_absolu(2700,1600);


    while (1) {
        if (gpio_get(GPIOA, GPIO0)) {
            // gpio_set  (GPIOG,  GPIO14);
        } else {
            // gpio_clear(GPIOG,  GPIO14);

        }

    }
}


int main() {
    main_blink();
}
