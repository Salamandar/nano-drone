#include "hardware/hardware.h"
#include "hardware/leds.h"
#include "hardware/motors.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"


extern void xPortSysTickHandler(void);
int count = 0;

void sys_tick_handler(void) {
    count++;
    // We call this handler every 1ms so 1000ms = 1s on/off.
    if (count == 100) {
        toggle_leds_rouges();
        count = 0;
    }
    xPortSysTickHandler();
}


//TASK 1: Toggle LED via RTOS Timer
void ToggleLEDRouges_Timer(void *pvParameters){
    while (1) {
        toggle_leds_rouges();

        // Delay for a period of time. vTaskDelay() places the task into
        // the Blocked state until the period has expired.
        // The delay period is spacified in 'ticks'. We can convert
        // this in milisecond with the constant portTICK_RATE_MS.
        vTaskDelay(100/portTICK_RATE_MS);
    }
}
//TASK 2: Toggle LED via RTOS Timer
void ToggleLEDBleues_Timer(void *pvParameters){
    while (1) {
        toggle_leds_bleues();
        vTaskDelay(150/portTICK_RATE_MS);
    }
}


xQueueHandle pbq;

int main() {
    init_hardware();

    // pbq = xQueueCreate(5, sizeof(int));
    // if (pbq == 0) {
    //     while(1); // fatal error
    // }

    // Create tasks
    TaskHandle_t *taskRouges, *taskBleues;
    xTaskCreate(
        ToggleLEDRouges_Timer,      // Function pointer
        "Task1",                    // Task name - for debugging only
        configMINIMAL_STACK_SIZE,   // Stack depth in words
        (void*) NULL,               // Pointer to tasks arguments (parameter)
        tskIDLE_PRIORITY + 1,       // Task priority
        taskRouges);                // Task handle

    vTaskDelete(taskRouges);
    toggle_leds_bleues();

    // xTaskCreate(
    //     ToggleLEDBleues_Timer,
    //     "Task2",
    //     configMINIMAL_STACK_SIZE,
    //     (void*) NULL,
    //     tskIDLE_PRIORITY + 1,
    //     taskBleues);

    // Start the RTOS Scheduler
    vTaskStartScheduler();
    while(1){};



    // int moteur = 0;

    // motor_set_speed(moteur*3, 0.3);
    // while(1) {
    //     delay_nop(1000000);

    //     // motor_set_speed(moteur*3, 0);
    //     // moteur = (moteur + 1)%2;
    //     // motor_set_speed(moteur*3, 0.3);

    //     toggle_leds_rouges();
    //     toggle_leds_bleues();


    // }
}
