#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "motor.h"
#include "encoder.h"
#include "switch.h" 
#include "I2C.h"

int motor_pos[8] = {45, 70, 45, 70, 45, 70, 45, 70};
int encoder_pos[8];

int last_sent[4];

int coeff[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int pr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void init_position();
int comp_pos(int *a, int *b);

void app_main(void) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_task_wdt_deinit();
    
    motor_init();
    encoder_init();
    i2c_init();
    // for(int i = 0; i < 8; i++) {
    //     motor_pos[i] = 0;
    // }
    
    init_position();

    printf("---------- Finished Initialization ---------- \n\n");

    int print_count = 0;
    int print_interv = 2000;

    int counter = 0;
    int state = 0;

    while (1) {
        // Refresh all the sensor readings
        read_switches();
        update_positions(motor_pos);
        get_counts(encoder_pos);

        

        move_positions(encoder_pos);
        update_positions(motor_pos);
        // for (int i = 0; i < 4; i++)
        // {
        //     if (read_switch(i)) {
        //         coeff[i] = encoder_pos[i];
        //     }
        //     pr[i] = encoder_pos[i] - coeff[i];
        // }

        // // Run anything in the main loop
        // printf("%d %d %d %d %d %d %d %d\n", (int)pr[0], (int)pr[1], (int)pr[2], (int)pr[3], (int)pr[4], (int)pr[5], (int)pr[6], (int)pr[7]);
        // printf("%x \n", get_switches());

        // Some delay stuff
        if(state == 0) {
            motor_pos[0] = 45;
            motor_pos[2] = 45;
        } else {
            motor_pos[0] = 0;
            motor_pos[2] = 0;
        }

        if (counter >= 1000) {
            if(state == 0) {
                state = 1;
            } else {
                state = 0;
            }
            counter = 0;
        }

        counter++;
        // vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

int comp_pos(int *a, int*b) {
    return (a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]) || (a[3] != b[3]);
}

void init_position() {
    home_motor_sw(0);
    // goto_pos(0, 15);
    // home_motor_sw(1);
    // goto_pos(1, 70);
    home_motor_sw(2);
    // goto_pos(2, 15);
    // home_motor_sw(3);
    // goto_pos(3, 70);

    // reset_encoder(0);
    // reset_encoder(1);
    // reset_encoder(2);
    // reset_encoder(3);
}
