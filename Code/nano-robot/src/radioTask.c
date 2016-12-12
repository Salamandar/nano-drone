#include "radioTask.h"

#include "hardware/hardware.h"
#include "hardware/radio.h"
#include "hardware/leds.h"
#include "hardware/motors.h"

#include "asservissement.h"

RadioConfig radioConfig = {
    .flip           = 0,
    .calibration_B  = 0,
    .calibration_F  = 0,
    .calibration_L  = 0,
    .calibration_R  = 0,
    .difficulty     = 1,
    .photo          = 0,
    .video          = 0,

    .radioState = None,
    .radioSignal_lostSince = 0,
};

// The variables are needed to detect rising/falling edges
// to convert "toggle buttons" to "push buttons"
// FirstIter prevents events on the first loop.
uint8_t buf[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool firstIter = true;
uint8_t buf13 = 0;
uint8_t buf14 = 0;

void receive_radio() {
    bool data_received = nrf_receive_data(15, buf);

    switch(radioConfig.radioState) {
    case None:
        nrf_setChann(25); // Debug purposes
        radioConfig.radioState = FirstHandShakeLost;
        if (data_received) {
            radioConfig.radioState = FirstHandShake;
            // set leds frequency first handshake
            set_leds_rouges();
        }
        return;

    case FirstHandShake:
        if (data_received) {
            // Nothing to do
        } else {
            radioConfig.radioState = FirstHandShakeLost;
            // Change channel now
            nrf_setChann(25);
            // set leds frequency first handshake lost
            set_leds_bleues();
            clear_leds_rouges();
        }
        return;

    case MainLost:
        radioConfig.radioSignal_lostSince+=5;
        // TODO manage signal loss
    case FirstHandShakeLost:
        if (data_received) {
            radioConfig.radioState = Main;
            // set leds frequency main
            set_leds_bleues();
            // Reset loss counter
            radioConfig.radioSignal_lostSince = 0;
        } else {
            return;
        }
        break;

    case Main:
        if (data_received) {
            // Nothing to do yet
        } else {
            radioConfig.radioState = MainLost;
            // set leds frequency main lost
            clear_leds_bleues();
            return;
        }
        break;
    }

    // Decode received data

    // Received : min 1000, max 2000
    radioConfig.cote = buf[ 5] + ((buf[ 6] & 0x0F) << 8) - 1000;
    radioConfig.avar = buf[ 7] + ((buf[ 8] & 0x0F) << 8) - 1000;
    radioConfig.gaz  = buf[ 9] + ((buf[10] & 0x0F) << 8) - 1000;
    radioConfig.rot  = buf[11] + ((buf[12] & 0x0F) << 8) - 1000;

    // buf_xx_ represents the changes in the buf[xx] data
    uint8_t buf_13_ = buf13^buf[13];
    buf13 = buf[13];
    uint8_t buf_14_ = buf14^buf[14];
    buf14 = buf[14];

    if (firstIter) {
        firstIter = false;
        return;
    }

    radioConfig.flip           = (buf[12] >> 4) & 0x01;
    radioConfig.calibration_B  = (buf_13_ >> 6) & 0x01;
    radioConfig.calibration_F  = (buf_13_ >> 5) & 0x01;
    radioConfig.calibration_L  = (buf_13_ >> 4) & 0x01;
    radioConfig.calibration_R  = (buf_13_ >> 3) & 0x01;
    radioConfig.difficulty     = (buf[13] >> 0) & 0x03;
    radioConfig.photo          = (buf_14_ >> 6) & 0x01;
    radioConfig.video          = (buf_14_ >> 7) & 0x01;

    // Test values
    clear_leds_bleues();
    clear_leds_rouges();

    // if (radioConfig.video) {
    //     set_leds_rouges();
    // }

    if        (radioConfig.calibration_F > 0) {
        set_leds_rouges();
        PIDpitch.P -=5;
        PIDroll .P -=5;
    } else if (radioConfig.calibration_B > 0) {
        set_leds_rouges();
        PIDpitch.P +=5;
        PIDroll .P +=5;
    } else if (radioConfig.calibration_L > 0) {
        set_leds_rouges();
        PIDpitch.I +=1;
        PIDroll .I +=1;
    } else if (radioConfig.calibration_R > 0) {
        set_leds_rouges();
        PIDpitch.I -=1;
        PIDroll .I -=1;
    }
}
