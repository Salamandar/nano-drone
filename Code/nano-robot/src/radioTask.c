#include <stdbool.h>
#include <stdint.h>

#include "hardware/hardware.h"
#include "hardware/radio.h"
#include "hardware/leds.h"

// Useful ?
enum ReceptionState{
    None,               // Initial
    FirstHandShake,     // Channel 2  detected! Should change channel soon.
    FirstHandShakeLost, // Channel 2  lost!     Should change channel now.
    Main,               // Channel 25 detected!
    MainLost,           // Channel 25 lost !

} receptionState = None;

uint8_t buf[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int mainSignal_lostSince = 0;

void receive_radio() {
    if (nrf_receive_data(15, buf)) {
        // Signal found !
        switch(receptionState) {
        case None:
            receptionState = FirstHandShake;
            // set leds frequency first handshake
            set_leds_rouges();
            break;

        case FirstHandShakeLost:
        case MainLost:
            receptionState = Main;
            // set leds frequency main
            set_leds_bleues();
            // Reset loss counter
            mainSignal_lostSince = 0;
            break;

        default:
            break;
        }
    } else {
        // Signal lost !
        switch(receptionState) {
        case FirstHandShake:
            receptionState = FirstHandShakeLost;
            // Change channel now
            nrf_setChann(25);
            // set leds frequency first handshake lost
            set_leds_bleues();
            clear_leds_rouges();
            break;

        case Main:
            receptionState = MainLost;
            // set leds frequency main lost
            clear_leds_bleues();
            break;

        default:
            break;
        }
    }
    delay_nop(80000);

    switch(receptionState) {
    case None:
    case FirstHandShake:
    case FirstHandShakeLost:
        return; // No main signal here

    case MainLost:
        mainSignal_lostSince++;
        return; // TODO manage signal loss

    case Main:
        break;
    }



    // Decode received data

    // Received : min 1000, max 2000
    int16_t cote = buf[ 5] + ((buf[ 6] & 0x0F) << 8) - 1000;
    int16_t avar = buf[ 7] + ((buf[ 8] & 0x0F) << 8) - 1000;
    int16_t gaz  = buf[ 9] + ((buf[10] & 0x0F) << 8) - 1000;
    int16_t rot  = buf[11] + ((buf[12] & 0x0F) << 8) - 1000;

    bool flip           = (buf[12] >> 4) & 0x01;
    bool calibration_B  = (buf[13] >> 6) & 0x01;
    bool calibration_F  = (buf[13] >> 5) & 0x01;
    bool calibration_L  = (buf[13] >> 4) & 0x01;
    bool calibration_R  = (buf[13] >> 3) & 0x01;
    int  difficulty     = (buf[13])      & 0x03;
    bool photo          = (buf[14] >> 6) & 0x01;
    bool video          = (buf[14] >> 7) & 0x01;



    // Test values
    clear_leds_bleues();
    clear_leds_rouges();

    switch (difficulty) {
    case 0:
        set_leds_rouges();
        break;
    case 1:
        set_leds_bleues();
        break;
    case 2:
        set_leds_rouges();
        set_leds_bleues();
        break;
    case 3:
        break;
    }


    // if (video) {
    //     set_leds_rouges();
    // } else {
    //     clear_leds_rouges();
    // }

    // motor_set_speed(0, (500 - cote)/2);
    // motor_set_speed(1, (500 - avar)/2);
    // motor_set_speed(2, (rot - 500)/2);

    motor_set_speed(0, gaz/4);
    motor_set_speed(1, gaz/4);
    motor_set_speed(2, gaz/4);
    motor_set_speed(3, gaz/4);

    // toggle_leds_bleues();



}
