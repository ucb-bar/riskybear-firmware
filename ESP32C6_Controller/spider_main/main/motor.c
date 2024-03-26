#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "motor.h"
#include "encoder.h"
#include "switch.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY (8000) // Frequency in Hertz. Set frequency at 4 kHz
#define MAX_SPEED 8000
#define MIN_SPEED 3000

/*  Motor 0: UPPER FRONT
    Motor 1: LOWER FRONT
    Motor 2: UPPER BACK
    Motor 3: LOWER BACK
*/

ledc_channel_t CHANNELS[4] = {LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3};
int PWM_PINS[4] = {GPIO_NUM_3, GPIO_NUM_0, GPIO_NUM_4, GPIO_NUM_6};
int DIR_PINS[4] = {GPIO_NUM_2, GPIO_NUM_1, GPIO_NUM_5, GPIO_NUM_7};

int DIR[4] = {1, -1, 1, -1};

int MAX_VALUES[4] = {65, 52, 100, 52};
int MIN_VALUES[4] = {-12, -63, -12, -63};

int motor_positions[4] = {0, 0, 0, 0};

float Kp = 150;
float Ki = 20;
float Kd = -1;


float MAX_SAT = 1000;

uint64_t last_time[4] = {0, 0, 0, 0};
float error_i[4] = {0, 0, 0, 0};
int last_pos[4] = {0, 0, 0, 0};

// ---------------------------------
// Setup Functions
// ---------------------------------

void init_single_motor(int num) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = CHANNELS[num],
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PWM_PINS[num],
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    gpio_reset_pin(DIR_PINS[num]);
    gpio_set_direction(DIR_PINS[num], GPIO_MODE_OUTPUT);
}

void motor_init(void) {
    init_single_motor(0);
    init_single_motor(1);
    init_single_motor(2);
    init_single_motor(3);

    printf("Motor Setup done\n");
}

// ---------------------------------
// Movement Functions
// ---------------------------------

void update_single_pos(int pos, int num)
{
    // pos = pos > MAX_VALUES[num] ? MAX_VALUES[num] : pos;
    // pos = pos < MIN_VALUES[num] ? MIN_VALUES[num] : pos;
    motor_positions[num] = pos;
}

void update_positions(int* pos) {
    update_single_pos(pos[0], 0);
    update_single_pos(pos[1], 1);
    update_single_pos(pos[2], 2);
    update_single_pos(pos[3], 3);
}

void set_motor(int num, float speed) {
    if (num >= 4) {
        printf("Invalid number for this ESPP\n");
        return ;
    }

    speed = DIR[num] * speed;
    speed = speed > MAX_SPEED ? MAX_SPEED : speed;
    speed = speed < -MAX_SPEED ? -MAX_SPEED : speed;

    speed = (speed > 0 && speed < MIN_SPEED) ? MIN_SPEED : speed;
    speed = (speed < 0 && speed > -MIN_SPEED) ? -MIN_SPEED : speed;

    if (speed > 0) {
        gpio_set_level(DIR_PINS[num], 0);
        ledc_set_duty(LEDC_MODE, CHANNELS[num], (int)speed);
    } else {
        gpio_set_level(DIR_PINS[num], 1);
        ledc_set_duty(LEDC_MODE, CHANNELS[num], MAX_SPEED + (int)speed);
    }
    ledc_update_duty(LEDC_MODE, CHANNELS[num]);
    
}

void move_pos(int num, int curr_pos) {
    if (num >= 4) {
        printf("Invalid number for this ESPP\n");
        return ;
    }

    // get current state
    uint64_t curr_time = esp_timer_get_time();
    int dt = (int) (curr_time - last_time[num]);

    // get errors
    float error_p = (float) (motor_positions[num] - curr_pos);
    float error_d =  ((float) (curr_pos - last_pos[num])) / dt;

    // calculate final output
    float pid_out = Kp * error_p + Kd * error_d;
    printf("error: %f, target: %d\n", error_p, motor_positions[num]);

    // update state
    last_time[num] = curr_time;
    last_pos[num] = curr_pos;

    set_motor(num, pid_out);
}

void goto_pos(int num, int target) {
    motor_positions[num] = target;
    int current = get_angle(num);
    int counter = 0;
    while (counter < 100) {
        current = get_angle(num);

        if ((motor_positions[num] == current) == 0) {
            counter++;
        } else {
            counter = 0;
        }
        move_pos(num, current);
    }
    set_motor(num, 0);
}

void move_positions(int *curr_poses)
{

    // printf("pain: %d %d %d %d\n", (int)motor_positions[0], (int)motor_positions[1], (int)motor_positions[2], (int)motor_positions[3]);
    move_pos(0, curr_poses[0]);
    // move_pos(1, curr_poses[1]);
    move_pos(2, curr_poses[2]);
    // move_pos(3, curr_poses[3]);
}

// ---------------------------------
// Homing Functions
// ---------------------------------

void home_motor_sw(int num) {
    printf("Homing motor: %d\n", num);

    int counter = 0;
    while (counter < 10) {
        set_motor(num, -1000);
        read_switches(); 
        if (read_switch(num)) {
            counter++;
        } else {
            counter = 0;
        }
        printf("%d\n", counter);
    }
    printf("Found button\n");

    while (read_switch(num)) {
        read_switches();
        set_motor(num, 1000);
    }

    set_motor(num, 0);
    reset_encoder(num);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    printf("Finished: %d\n", num);
}
