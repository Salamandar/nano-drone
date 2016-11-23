#include "ledsTask.h"
#include "hardware/leds.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"


void ToggleLEDRouges_Timer(void *pvParameters){
    while (1) {
        toggle_leds_rouges();
        vTaskDelay(400/portTICK_RATE_MS);
    }
}

void ToggleLEDBleues_Timer(void *pvParameters){
    int delay = 0;
    int sgn = 1;
    while (1) {
        toggle_leds_bleues();
        vTaskDelay(20-delay);
        toggle_leds_bleues();
        vTaskDelay(delay);
        delay += (1*sgn);
        if(delay >= 20)
            sgn = -1;
        if(delay <= 0)
            sgn = +1;
    }
}

void init_leds_task() {
    // Create tasks
    xTaskCreate(
        ToggleLEDRouges_Timer,      // Function pointer
        "Task1",                    // Task name - for debugging only
        configMINIMAL_STACK_SIZE,   // Stack depth in words
        (void*) NULL,               // Pointer to tasks arguments (parameter)
        tskIDLE_PRIORITY + 2,       // Task priority
        NULL);                      // Task handle

    xTaskCreate(
        ToggleLEDBleues_Timer,
        "Task2",
        configMINIMAL_STACK_SIZE,
        (void*) NULL,
        tskIDLE_PRIORITY + 2,
        NULL);
}
