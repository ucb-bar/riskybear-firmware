#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "math.h"
#include "esp_task_wdt.h"

#include "libraries/switch.h"
#include "libraries/I2C.h"
#include "libraries/UART_TSI.h"
#include "joints.h"
#include "controller.h"
#include "home.h"

void print_encoders();
void passthrough_speeds();
void passthrough_positions();

int motor_speeds[8];
int motor_positions[8];

void app_main(void) {
  printf("READY---\n");
  esp_task_wdt_deinit();
  vTaskDelay(1000 / portTICK_PERIOD_MS);



  // +------------------------------------------------+
  // | Initialize the controller
  // +------------------------------------------------+
  printf("[START INIT]\n");
  i2c_init();
  joints_init();



  // +------------------------------------------------+
  // | Start initial homing sequence
  // +------------------------------------------------+
  printf("[START HOME]\n");
  home_motors();

  motor_set_state(1);



  // +------------------------------------------------+
  // | Start main loop
  // +------------------------------------------------+
  printf("[START MAIN]\n");
  while (1) {
    // for (int i=0; i<10; i++) {
    //   print_encoders();
    //   vTaskDelay(300 / portTICK_PERIOD_MS);
    // }

    // for (int i = 0; i < 8; i++){
    //   motor_positions[i] = 450;
    // }
    // passthrough_positions();

    // for (int i=0; i<10; i++) {
    //   print_encoders();
    //   vTaskDelay(300 / portTICK_PERIOD_MS);
    // }

    // for (int i = 0; i < 8; i++){
    //   motor_positions[i] = 0;
    // }
    // passthrough_positions();

    step();
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}


void print_encoders() {
  int enc[8];

  for (int i = 0; i < 8; i++) {
    enc[i] = get_encoder(i);
  }

  printf("[%d, %d, %d, %d, %d, %d, %d, %d]\n", enc[0], enc[1], enc[2], enc[3], enc[4], enc[5], enc[6], enc[7]);
}

void passthrough_speeds() {
  for (int i = 0; i < 8; i++) {
    set_motor_speed(i, motor_speeds[i]);
  }
}

void passthrough_positions() {
  for (int i = 0; i < 8; i++) {
    set_motor_pos(i, motor_positions[i]);
  }
}
