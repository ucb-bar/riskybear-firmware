#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"

#include "rotary_encoder.h"
#include "encoder.h"

#define ENABLE_HALF_STEPS true
#define RESET_AT 0
#define FLIP_DIRECTION false

rotary_encoder_info_t ENCODERS[4] = {{0}, {0}, {0}, {0}};
int ENC_B[4] = {GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_11, GPIO_NUM_18};
int ENC_A[4] = {GPIO_NUM_23, GPIO_NUM_20, GPIO_NUM_15, GPIO_NUM_19};


// int ENC_A[4] = {0, 0, 0, 0};
// int ENC_B[4] = {0, 0, 0, 0};

void init_single_encoder(int num) {
    ESP_ERROR_CHECK(rotary_encoder_init(&ENCODERS[num], ENC_A[num], ENC_B[num]));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&ENCODERS[num], ENABLE_HALF_STEPS));

    QueueHandle_t event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&ENCODERS[num], event_queue));
}


void encoder_init() {
    gpio_install_isr_service(0);
    init_single_encoder(0);
    init_single_encoder(1);
    init_single_encoder(2);
    init_single_encoder(3);

    printf("Encoder Setup done\n");
}

void reset_encoder(int num) {
    rotary_encoder_reset(&ENCODERS[num]);
}

void get_counts(int8_t *enc_pos)
{
    enc_pos[0] = get_angle(0);
    enc_pos[1] = get_angle(1);
    enc_pos[2] = get_angle(2);
    enc_pos[3] = get_angle(3);
}

int get_count(int num) {
    rotary_encoder_state_t state = {0};
    ESP_ERROR_CHECK(rotary_encoder_get_state(&ENCODERS[num], &state));

    return state.position;
}

int8_t get_angle(int num)
{
    rotary_encoder_state_t state = {0};
    ESP_ERROR_CHECK(rotary_encoder_get_state(&ENCODERS[num], &state));

    if (num == 2) {
        return (state.position * 360) / (6 * 100);
    }

    return (state.position * 360) / (6 * 150);
}
