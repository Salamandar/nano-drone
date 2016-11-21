#include "hardware/hardware.h"
#include "hardware/leds.h"
#include "hardware/motors.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

//TASKs : Toggle LED via RTOS Timer
void ToggleLEDRouges_Timer(void *pvParameters){
    while (1) {
        toggle_leds_rouges();
        vTaskDelay(400/portTICK_RATE_MS);
    }
}
//TASK 2: Toggle LED via RTOS Timer
void ToggleLEDBleues_Timer(void *pvParameters){
    while (1) {
        toggle_leds_bleues();
        vTaskDelay(75/portTICK_RATE_MS);
    }
}


xQueueHandle pbq;

int main() {
    init_hardware();

    pbq = xQueueCreate(5, sizeof(int));
    if (pbq == 0) {
        while(1); // fatal error
    }

    // Create tasks
    xTaskCreate(
        ToggleLEDRouges_Timer,      // Function pointer
        "Task1",                    // Task name - for debugging only
        configMINIMAL_STACK_SIZE,   // Stack depth in words
        (void*) NULL,               // Pointer to tasks arguments (parameter)
        tskIDLE_PRIORITY + 1,       // Task priority
        NULL);                      // Task handle

    xTaskCreate(
        ToggleLEDBleues_Timer,
        "Task2",
        configMINIMAL_STACK_SIZE,
        (void*) NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    // Start the RTOS Scheduler
    vTaskStartScheduler();
    while(1);
}
