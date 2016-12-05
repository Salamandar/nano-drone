#pragma once

#include "pid.h"


void init_asservissement();

void asservissement_update    (int angleYaw, int anglePitch, int angleRoll, int dt);
void asservissement_setConsign(int angleYaw, int anglePitch, int angleRoll, int throttle);

void asservissement_getMotorSpeed(int *motorFL,
                                  int *motorFR,
                                  int *motorBL,
                                  int *motorBR);

// For tuning purposes
extern PID PIDyaw;
extern PID PIDpitch;
extern PID PIDroll;
