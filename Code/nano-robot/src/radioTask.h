#pragma once

#include <stdbool.h>
#include <stdint.h>

void receive_radio();

typedef enum ReceptionState{
    None,               // Initial
    FirstHandShake,     // Channel 2  detected! Should change channel soon.
    FirstHandShakeLost, // Channel 2  lost!     Should change channel now.
    Main,               // Channel 25 detected!
    MainLost,           // Channel 25 lost !
} ReceptionState;

typedef struct RadioConfig {
    int16_t cote;
    int16_t avar;
    int16_t gaz;
    int16_t rot;

    bool flip;
    bool calibration_B;
    bool calibration_F;
    bool calibration_L;
    bool calibration_R;
    int  difficulty;
    bool photo;
    bool video;

    ReceptionState radioState;
    int radioSignal_lostSince;
} RadioConfig;

extern RadioConfig radioConfig;
