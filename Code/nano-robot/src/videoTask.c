#include "videoTask.h"
#include "hardware/video.h"
#include "hardware/leds.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

// * Durée Photo : ([460 | 550] - 230ms) = 230-320ms
#define DELAY_PHOTO 300
// * Durée Vidéo : ([840 | 940] - 230ms) = 610-710ms
#define DELAY_VIDEO 700

SemaphoreHandle_t   videoSemaphore = NULL;

volatile enum VideoSignalType {
    None,
    Photo,
    Video,
} videoSignalType;

// TODO better handle multiple events ?

void videoTaskFunction(void *pvParameters) {
    while(1) {
        // Maybe another event occured already ?
        if(videoSignalType == None)
            xSemaphoreTake(videoSemaphore, portMAX_DELAY);
        set_video_pin();
        //set_leds_rouges();
        switch(videoSignalType) {
        case None:
            break;
        case Photo:
            videoSignalType = None;
            vTaskDelay(DELAY_PHOTO);
            break;
        case Video:
            videoSignalType = None;
            vTaskDelay(DELAY_VIDEO);
            break;
        }
        clear_video_pin();
        // clear_leds_rouges();
        vTaskDelay(10/portTICK_RATE_MS);
    }
}

void debugSemaphoreFunction(void *pvParameters) {
    while (1) {
        photo_signal();
        vTaskDelay(2000/portTICK_RATE_MS);
        video_signal();
        vTaskDelay(2000/portTICK_RATE_MS);
    }
}

void init_video_task() {
    vSemaphoreCreateBinary(videoSemaphore);
    xSemaphoreTake        (videoSemaphore, 0);

    xTaskCreate(
        videoTaskFunction,          // Function pointer
        "TaskVideo",                // Task name - for debugging only
        configMINIMAL_STACK_SIZE,   // Stack depth in words
        (void*) NULL,               // Pointer to tasks arguments (parameter)
        tskIDLE_PRIORITY + 1,       // Task priority
        NULL);                      // Task handle


    // Debug Semaphore
    xTaskCreate(
        debugSemaphoreFunction,
        "TaskSem",
        configMINIMAL_STACK_SIZE,
        (void*) NULL,
        tskIDLE_PRIORITY + 2,
        NULL);
}


void photo_signal() {
    videoSignalType = Photo;
    xSemaphoreGive(videoSemaphore);
}

void video_signal() {
    videoSignalType = Video;
    xSemaphoreGive(videoSemaphore);
}
