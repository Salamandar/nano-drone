#include "pid.h"

#include "hardware/leds.h"

void set_PID_input(PID *pid, int value, int dt) {
    pid->error_p = pid->consign - value;
    pid->error_i = (pid->error_i + pid->error_p * dt) / 2;
    pid->error_d = (pid->error_p - pid->error_last) / dt;

    pid->error_last = pid->error_p;
}

int get_PID_output(PID *pid) {
    // if (pid->P * pid->error_p < 0
    //  || pid->I * pid->error_i < 0
    //  || pid->D * pid->error_d < 0
    //  ) {
    //     set_leds_rouges();
    // }


    return pid->P * pid->error_p
         + pid->I * pid->error_i
         + pid->D * pid->error_d;
}
