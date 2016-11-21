#include "hardware/hardware.h"
#include "hardware/leds.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>


int main() {
    init_hardware();

    set_leds_rouges();

    int moteur = 0;

    motor_set_speed(3, 0.3);
    while(1) {
        delay_nop(1000000);

        motor_set_speed(moteur, 0);
        moteur = (moteur + 1)%4;
        motor_set_speed(moteur, 0.3);

        toggle_leds_rouges();
        toggle_leds_bleues();


    }

}

    // Mot_Avant_gauche,
    // Mot_Avant_droite,
    // Mot_Arrie_gauche,
    // Mot_Arrie_droite,
