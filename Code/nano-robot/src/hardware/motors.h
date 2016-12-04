#pragma once

#include <inttypes.h>

#define PWM_PRESCALE        (1)
#define PWM_PERIOD          (1024)

#include <libopencm3/stm32/timer.h>

typedef enum _Motor {
    Mot_Arrie_droite = TIM_OC1,
    Mot_Avant_droite = TIM_OC2,
    Mot_Avant_gauche = TIM_OC3,
    Mot_Arrie_gauche = TIM_OC4,
} Motor;

void init_motors();

// Speed between 0 and PWM_PERIOD
#define motor_max PWM_PERIOD - 1
#define motor_min 10

void motor_set_speed(Motor motor, uint32_t speed);
