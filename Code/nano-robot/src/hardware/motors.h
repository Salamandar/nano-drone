#pragma once

#include <inttypes.h>

#define PWM_PRESCALE        (1)
#define PWM_PERIOD          (256)


typedef enum _Motor {
    Mot_Avant_gauche = 0,
    Mot_Avant_droite = 1,
    Mot_Arrie_gauche = 2,
    Mot_Arrie_droite = 3,
} Motor;

void init_motors();

// Speed between 0 and PWM_PERIOD
void motor_set_speed(Motor motor, uint8_t speed);
