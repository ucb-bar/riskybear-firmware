#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "libraries/switch.h"
#include "joints.h"
#include "controller.h"

#define F_LOW 100
#define F_HIGH 950

#define F_FRONT 20
#define F_BACK 800

#define B_FRONT 800
#define B_BACK 20

int step_fwd[6][8] = {
    {F_LOW, F_FRONT, F_HIGH, B_BACK, F_BACK, F_HIGH, B_FRONT, F_LOW},
    {F_LOW, F_BACK, F_HIGH, B_FRONT, F_FRONT, F_HIGH, B_BACK, F_LOW},
    {F_LOW, F_BACK, F_LOW, B_FRONT, F_FRONT, F_LOW, B_BACK, F_LOW},
    {F_HIGH, F_BACK, F_LOW, B_FRONT, F_FRONT, F_LOW, B_BACK, F_HIGH},
    {F_HIGH, F_FRONT, F_LOW, B_BACK, F_BACK, F_LOW, B_FRONT, F_HIGH},
    {F_LOW, F_FRONT, F_LOW, B_BACK, F_BACK, F_LOW, B_FRONT, F_LOW},
};

void step() {
  for(int t = 0; t < 6; t++) {
    for(int i = 0; i < 8; i++) {
      set_motor_pos(i, step_fwd[t][i]);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

}
