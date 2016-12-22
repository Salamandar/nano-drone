#include "hardware/hardware.h"
#include "hardware/eeprom.h"
#include "hardware/leds.h"
#include "hardware/motors.h"
#include "hardware/mpu.h"
#include "ledsTask.h"
#include "radioTask.h"
#include "videoTask.h"

#include "music.h"

#include "asservissement.h"
#include "math_utils.h"

#include <math.h>

int angleYaw = 0, anglePitch, angleRoll;
int motorFL, motorFR, motorBL, motorBR;


int main() {
    init_hardware();
    // eeprom_test();
    init_asservissement();
    asservissement_setConsign(0, 0, 0, 00);

    imperial_march();


    while (1) {
        receive_radio();

        asservissement_setThrottle(radioConfig.gaz);

        mpu_get_inertial_values();

        // complementaryFilter(
        //         (fast_atan2(accel.y, -accel.z) - 90)*100,
        //         gyro.x, 1, &anglePitch);
        // complementaryFilter(
        //         (fast_atan2(accel.x, -accel.z) - 90)*100,
        //         gyro.y, 1, &angleRoll);

        // En degrés
        angleYaw   = - gyro.z/8;
        anglePitch = fast_atan2(accel.a/10, -accel.z/10) - 90;
        angleRoll  = fast_atan2(accel.b/10, -accel.z/10) - 90;

        asservissement_update(angleYaw, anglePitch, angleRoll, 1);

        asservissement_getMotorSpeed(&motorFL, &motorFR, &motorBL, &motorBR);

        motor_set_speed(Mot_Avant_gauche, motorFL);
        // motor_set_speed(Mot_Avant_droite, motorFR);
        // motor_set_speed(Mot_Arrie_gauche, motorBL);
        motor_set_speed(Mot_Arrie_droite, motorBR);
    }

    // init_leds_task();
    // init_video_task();
    // xTaskCreate(
    //     test,
    //     "Task2",
    //     configMINIMAL_STACK_SIZE,
    //     (void*) NULL,
    //     tskIDLE_PRIORITY + 2,
    //     NULL);
    // // xTaskCreate(
    // //     test2,
    // //     "Task2",
    // //     configMINIMAL_STACK_SIZE,
    // //     (void*) NULL,
    // //     tskIDLE_PRIORITY + 2,
    // //     NULL);

    // toggle_leds_rouges();

    // // Start the RTOS Scheduler
    // vTaskStartScheduler();
    while(1);
}
