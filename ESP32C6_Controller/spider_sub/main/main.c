#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/i2c_slave.h"

#include "motor.h"
#include "encoder.h"

#define I2C_SLAVE_SCL_IO GPIO_NUM_12
#define I2C_SLAVE_SDA_IO GPIO_NUM_13

#define I2C_ADDR 0x58
#define RD_LENGTH 4
#define WR_LENGTH 4

int8_t *data_rd;
int8_t *encoder_pos;

int rcv_rdy = 0;

i2c_slave_dev_handle_t slave_handle;
QueueHandle_t s_receive_queue;
i2c_slave_rx_done_event_data_t rx_data;


static IRAM_ATTR bool i2c_slave_rx_done_callback(i2c_slave_dev_handle_t channel, const i2c_slave_rx_done_event_data_t *edata, void *user_data) {
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

void i2c_setup();
static void i2c_task(void *arg);
void init_position();

void app_main(void) {
    esp_task_wdt_deinit();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("Starting setup\n");

    motor_init();
    encoder_init();
    i2c_setup();

    for(int i = 0; i < 4; i++) {
        data_rd[i] = 0;
    }
    // init_position();

    xTaskCreate(i2c_task, "i2c_task", 4096, NULL, 2, NULL);

    printf("---------- Finished Initialization ---------- \n\n");
    int print_count = 0;
    int print_interv = 100;
    while (1) {
        get_counts(encoder_pos);
        // move_positions(encoder_pos);
        // update_positions(data_rd);
        // printf("data: %d %d %d %d\n", (int)data_rd[0], (int)data_rd[1], (int)data_rd[2], (int)data_rd[3]);
        // update_positions(data_rd);

        // Run anything in the main loop
        // if (print_count >= print_interv) {
        printf("encoders: %d %d %d %d\n", (int)encoder_pos[0], (int)encoder_pos[1], (int)encoder_pos[2], (int)encoder_pos[3]);
        //     print_count = 0;
        // } else {
        //     print_count++;
        // }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void i2c_setup() {
    i2c_slave_config_t i2c_config = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .send_buf_depth = 256,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .slave_addr = I2C_ADDR,
    }; // I have no idea wht TF I need this delay

    data_rd = (int8_t *)malloc(RD_LENGTH);
    encoder_pos = (int8_t *)malloc(WR_LENGTH);
    for (int i = 0; i < WR_LENGTH; i++)
    {
        encoder_pos[i] = i + 1;
    }
    // vTaskDelay(1000 / portTICK_PERIOD_MS); // I have no idea wht TF I need this delay
    ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_config, &slave_handle));

    s_receive_queue = xQueueCreate(4, sizeof(i2c_slave_rx_done_event_data_t));
    i2c_slave_event_callbacks_t cbs = {
        .on_recv_done = i2c_slave_rx_done_callback,
    };
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_handle, &cbs, s_receive_queue));

}

static void i2c_task(void *arg) {
    while (true) {
        ESP_ERROR_CHECK(i2c_slave_receive(slave_handle, (uint8_t *) data_rd, RD_LENGTH));
        xQueueReceive(s_receive_queue, &rx_data, pdMS_TO_TICKS(10000));
        ESP_ERROR_CHECK(i2c_slave_transmit(slave_handle, (uint8_t *) encoder_pos, 4, -1));
        // vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    // Exit
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(i2c_del_slave_device(slave_handle));

    vTaskDelete(NULL);
}

void init_position() {
    home_motor_enc(0);
    goto_pos(0, 14);
    home_motor_enc(1);
    goto_pos(1, 70);
    home_motor_enc(2);
    goto_pos(2, 15);
    home_motor_enc(3);
    goto_pos(3, 70);

    reset_encoder(0);
    reset_encoder(1);
    reset_encoder(2);
    reset_encoder(3);
}



