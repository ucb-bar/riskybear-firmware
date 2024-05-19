#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "libraries/UART_TSI.h"
#include "joints.h"

#define BASE_ADDR 0x13000000

#define MOTOR_TARGET        BASE_ADDR + 0x00
#define JOINT_STATE         BASE_ADDR + 0x04
#define JOINT_EN            BASE_ADDR + 0x08
#define JOINT_DIR           BASE_ADDR + 0x0C

#define MOTOR_SPEED         BASE_ADDR + 0x20
#define MOTOR_PRESC         BASE_ADDR + 0x24

#define ENCODER_COUNT       BASE_ADDR + 0x30
#define ENCODER_RESET       BASE_ADDR + 0x34

int DIR[8] = {0, 1, 0, 1, 1, 0, 1, 0};
int MOVE_DIR[8] = {1, -1, -1, 1, 1, -1, -1, 1};

void joints_init(void) {
  UART_TSI_init();

  for(int i=0; i<8; i++) {
    // Set motor pre-scaler and direction
    UART_TSI_write(MOTOR_PRESC + (0x100 * i), 200);    
    UART_TSI_write(JOINT_DIR + (0x100 * i), DIR[i]);

    // Set targets/speeds to 0
    UART_TSI_write(MOTOR_TARGET + (0x100 * i), 0);
    UART_TSI_write(MOTOR_SPEED + (0x100 * i), 0);

    // Enable motors
    UART_TSI_write(JOINT_EN + (0x100 * i), 1);          
  }

  // We reset the encoder values
  reset_encoders();
}

// Reset encoder counter values
void reset_encoders(void) {
  for (int i = 0; i < 8; i++) {
    UART_TSI_write(ENCODER_RESET + (0x100 * i), (uint32_t) 1);
  }
}

// Reset encoder counter values
void reset_encoder(int num) {
  UART_TSI_write(ENCODER_RESET + (0x100 * num), (uint32_t)1);
}

// Change the motor enable
void motor_set_en(int state) {
  for (int i = 0; i < 8; i++) {
    UART_TSI_write(JOINT_EN + (0x100 * i), (uint32_t) state);
  }
}

// Change the motor control state
void motor_set_state(int state) {
  for (int i = 0; i < 8; i++) {
    UART_TSI_write(JOINT_STATE + (0x100 * i), (uint32_t) state);
  }
}

// Set a single motor speed
void set_motor_speed(int num, int speed) {
  UART_TSI_write(MOTOR_SPEED + (0x100 * num), (uint32_t)(speed * MOVE_DIR[num]));
}

// Set a single motor target position
void set_motor_pos(int num, int pos) {
  UART_TSI_write(MOTOR_TARGET + (0x100 * num), (uint32_t)(pos * MOVE_DIR[num]));
}

// Get encoder value
int get_encoder(int num) {
  return ((int)UART_TSI_read(ENCODER_COUNT + (0x100 * num))) * MOVE_DIR[num];
}