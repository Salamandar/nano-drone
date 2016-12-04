#include "sixAxisTask.h"

#include "hardware/mpu.h"
#include "hardware/motors.h"

#include <stdlib.h>
#include <math.h>

#define COS45 1.41421356237/2

#define GAZ 0
#define P   1

void mpuTask() {
    mpu_get_inertial_values();

    int16_t axis02 = COS45*(accel.x+accel.y);
    int16_t axis13 = COS45*(accel.x-accel.y);

    if (axis02 > 0) {
        motor_set_speed(Mot_Avant_gauche, GAZ+axis02/(10*P));
        motor_set_speed(Mot_Arrie_droite, 0);
    }
    else {
        motor_set_speed(Mot_Arrie_droite, GAZ-axis02/(10*P));
        motor_set_speed(Mot_Avant_gauche, 0);
    }

    if (axis13 > 0) {
        motor_set_speed(Mot_Arrie_gauche, GAZ+axis13/(10*P));
        motor_set_speed(Mot_Avant_droite, 0);
    }
    else {
        motor_set_speed(Mot_Avant_droite, GAZ-axis13/(10*P));
        motor_set_speed(Mot_Arrie_gauche, 0);
    }
}
