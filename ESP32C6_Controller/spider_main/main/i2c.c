#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/i2c_master.h"

// #define I2C_MASTER_SCL_IO GPIO_NUM_1
// #define I2C_MASTER_SDA_IO GPIO_NUM_0

#define I2C_MASTER_SCL_IO GPIO_NUM_12
#define I2C_MASTER_SDA_IO GPIO_NUM_13

#define DEV_ADDR GPIO_NUM_13

#define IO_PORT 0x20
#define ESP_PORT 0x58
i2c_master_dev_handle_t io_handle;
i2c_master_dev_handle_t esp_handle;

void i2c_init(void) {
    i2c_master_bus_config_t i2c_mst_config_1 = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };
    i2c_master_bus_handle_t bus_handle;

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config_1, &bus_handle));
    ESP_ERROR_CHECK(i2c_master_probe(bus_handle, IO_PORT, -1));
    printf("IO Expander Found!\n");
    ESP_ERROR_CHECK(i2c_master_probe(bus_handle, ESP_PORT, -1));
    printf("ESP32 Found!\n");

    i2c_device_config_t iodev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = IO_PORT,
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &iodev_cfg, &io_handle));

    i2c_device_config_t espdev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ESP_PORT,
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &espdev_cfg, &esp_handle));


    printf("I2C Setup done\n");
}

uint8_t i2c_read_io() {
    uint8_t buf[20] = {0x00};
    uint8_t buffer[2];

    ESP_ERROR_CHECK(i2c_master_transmit_receive(io_handle, buf, sizeof(buf), buffer, 2, -1));
    return buffer[0];
}

void esp_WR_RD(uint8_t *motor_pos, uint8_t *encoder_pos) {
    ESP_ERROR_CHECK(i2c_master_transmit_receive(esp_handle, motor_pos, sizeof(motor_pos), encoder_pos, 4, -1));
}
