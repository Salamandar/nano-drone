#pragma once

typedef enum _Motor {
    Mot_Avant_gauche = 0,
    Mot_Avant_droite = 1,
    Mot_Arrie_gauche = 2,
    Mot_Arrie_droite = 3,
} Motor;

void init_motors();

void motor_set_speed(Motor motor, float speed);
