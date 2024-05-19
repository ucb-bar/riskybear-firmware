#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "UART_TSI.h"

#define  CMD_READ  0x00
#define  CMD_WRITE  0x01

#define UART_TSI_RX GPIO_NUM_15
#define UART_TSI_TX GPIO_NUM_11

#define UART UART_NUM_1

void UART_TSI_init(void) {
  printf("Start UART TSI Setup\n");

  uart_config_t uart_config = {
      .baud_rate = 921600,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };

  const int uart_buffer_size = (1024 * 2);
  QueueHandle_t uart_queue;

  // Configure UART parameters
  ESP_ERROR_CHECK(uart_driver_install(UART, uart_buffer_size, 0, 0, NULL, 0));
  ESP_ERROR_CHECK(uart_param_config(UART, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART, UART_TSI_TX, UART_TSI_RX,
                               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  printf("UART TSI Setup done\n");
}

int UART_TSI_read(uint64_t address) {
  uint8_t req[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  memcpy(req + 4, &address, 8);
  uart_write_bytes(UART, req, 20);

  uint8_t ret[4];
  uart_read_bytes(UART, ret, 4, portMAX_DELAY);

  int received_value;
  memcpy(&received_value, ret, sizeof(float));

  return received_value;
}

void UART_TSI_write(uint64_t address, uint32_t data) {
  uint8_t write[24] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  memcpy(write + 4, &address, 8);
  memcpy(write + 20, &data, 4);
  uart_write_bytes(UART, write, 24);
}