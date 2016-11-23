#include "video.h"

#include <libopencm3/stm32/gpio.h>

#define VIDEO_PORT  GPIOB
#define VIDEO_PIN   GPIO0

void init_video_pin() {
    gpio_mode_setup(VIDEO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, VIDEO_PIN);
    set_video_pin();
}
void   set_video_pin() { gpio_set   (VIDEO_PORT, VIDEO_PIN); }
void clear_video_pin() { gpio_clear (VIDEO_PORT, VIDEO_PIN); }
