#include "hardware/hardware.h"
#include "hardware/eeprom.h"
#include "hardware/leds.h"
#include "hardware/motors.h"
#include "hardware/mpu.h"
#include "ledsTask.h"
#include "radioTask.h"
#include "videoTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

xQueueHandle pbq;

void test(void* test) {
    while(1) {
        vTaskDelay(100);
    }
}
void test2(void* test) {
    while(1) {
        vTaskDelay(100);
    }
}

int main() {
    init_hardware();
    eeprom_test();
    while (1) {
        receive_radio();
        mpu_get_inertial_values();
    }


    pbq = xQueueCreate(5, sizeof(int));
    if (pbq == 0) {
        while(1); // fatal error
    }
    // for (int motor = 0; motor < 4; ++motor) {
    //     motor_set_speed(motor, 128);
    // }

    init_leds_task();
    init_video_task();
    xTaskCreate(
        test,
        "Task2",
        configMINIMAL_STACK_SIZE,
        (void*) NULL,
        tskIDLE_PRIORITY + 2,
        NULL);
    // xTaskCreate(
    //     test2,
    //     "Task2",
    //     configMINIMAL_STACK_SIZE,
    //     (void*) NULL,
    //     tskIDLE_PRIORITY + 2,
    //     NULL);

    toggle_leds_rouges();

    // Start the RTOS Scheduler
    vTaskStartScheduler();
    while(1);
}
