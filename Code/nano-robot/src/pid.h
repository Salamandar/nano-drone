#pragma once

typedef struct PID {
    int P;
    int I;
    int D;

    int consign;

    // Proportional
    int error_p;
    // Integration of the error
    int error_i;
    // Derivation of the error
    int error_d;
    // Last error to compute the derivation
    int error_last;

} PID;


void set_PID_input  (PID *pid, int value, int dt);
int  get_PID_output (PID *pid);
