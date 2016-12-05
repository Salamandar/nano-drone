#include "asservissement.h"

#include "hardware/mpu.h"
#include <stdlib.h>
#include <math.h>

#define COS45 1.41421356237/2

#define P_ 30
#define I_ 0
#define D_ 0



PID PIDyaw;
#define P_yaw       1
#define I_yaw       0
#define D_yaw       0

PID PIDpitch;
#define P_pitch     P_
#define I_pitch     I_
#define D_pitch     D_

PID PIDroll;
#define P_roll      P_
#define I_roll      I_
#define D_roll      D_

int m_angleYaw      = 0;
int m_anglePitch    = 0;
int m_angleRoll     = 0;

int m_throttle = 0;


void init_asservissement() {
    PIDyaw = (PID) {
        .P          = P_yaw,
        .I          = I_yaw,
        .D          = D_yaw,
        .consign    = 0,
        .error_p    = 0,
        .error_i    = 0,
        .error_d    = 0,
        .error_last = 0,
    };
    PIDpitch = (PID) {
        .P          = P_pitch,
        .I          = I_pitch,
        .D          = D_pitch,
        .consign    = 0,
        .error_p    = 0,
        .error_i    = 0,
        .error_d    = 0,
        .error_last = 0,
    };
    PIDroll = (PID) {
        .P          = P_roll,
        .I          = I_roll,
        .D          = D_roll,
        .consign    = 0,
        .error_p    = 0,
        .error_i    = 0,
        .error_d    = 0,
        .error_last = 0,
    };
}

void asservissement_update(int angleYaw, int anglePitch, int angleRoll, int dt) {
    set_PID_input(&PIDyaw,   angleYaw,   dt);
    set_PID_input(&PIDpitch, anglePitch, dt);
    set_PID_input(&PIDroll,  angleRoll,  dt);
}


void asservissement_setConsign(int angleYaw, int anglePitch, int angleRoll, int throttle) {
    PIDyaw  .consign = angleYaw;
    PIDpitch.consign = anglePitch;
    PIDroll .consign = angleRoll;
    m_throttle = throttle;
}

void asservissement_setThrottle(int throttle) {
    m_throttle = throttle;
}

#define diviser 1024

void asservissement_getMotorSpeed (int *motorFL,
                                   int *motorFR,
                                   int *motorBL,
                                   int *motorBR) {
    *motorFL = m_throttle + get_PID_output(&PIDyaw) + get_PID_output(&PIDpitch)/diviser + get_PID_output(&PIDroll)/diviser;
    *motorFR = m_throttle - get_PID_output(&PIDyaw) + get_PID_output(&PIDpitch)/diviser - get_PID_output(&PIDroll)/diviser;
    *motorBL = m_throttle - get_PID_output(&PIDyaw) - get_PID_output(&PIDpitch)/diviser + get_PID_output(&PIDroll)/diviser;
    *motorBR = m_throttle + get_PID_output(&PIDyaw) - get_PID_output(&PIDpitch)/diviser - get_PID_output(&PIDroll)/diviser;

}
