#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "motor.h"
#include "encoder.h"
#include "switch.h" 
#include "I2C.h"


uint8_t motor_pos[8] = {14, 70, 14, 70, 14, 70, 14, 70};
uint8_t encoder_pos[8];

uint8_t last_sent[4];

uint8_t coeff[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t pr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void init_position();
int comp_pos(uint8_t *a, uint8_t *b);

void app_main(void) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_task_wdt_deinit();
    
    motor_init();
    encoder_init();
    i2c_init();
    // for(int i = 0; i < 8; i++) {
    //     motor_pos[i] = 0;
    // }
    
    // init_position();

    printf("---------- Finished Initialization ---------- \n\n");

    int print_count = 0;
    int print_interv = 2000;

    while (1) {
        // Refresh all the sensor readings
        read_switches();
        update_positions(motor_pos);
        get_counts(encoder_pos);
        // vTaskDelay(100 / portTICK_PERIOD_MS);

        // move_positions(encoder_pos);

        // if (comp_pos(last_sent, motor_pos + 4)) {
        esp_WR_RD(motor_pos + 4, encoder_pos + 4);
        // }

        for (int i=0; i<8; i++) {
            if (i == 7) {
                if (read_switch(6)) {
                    coeff[i] = encoder_pos[i];
                }
            } else if (i == 6) {
                if (read_switch(7)) {
                    coeff[i] = encoder_pos[i];
                }    
            } else if (read_switch(i)) {
                coeff[i] = encoder_pos[i];
            }
            pr[i] = encoder_pos[i] - coeff[i];
        }

        // Run anything in the main loop
        // if (print_count >= print_interv) {
        printf("%d %d %d %d %d %d %d %d\n", (int)pr[0], (int)pr[1], (int)pr[2], (int)pr[3], (int)pr[4], (int)pr[5], (int)pr[6], (int)pr[7]);
        //     print_count = 0;
        // } else {
        //     print_count++;
        // }
            
        // Some delay stuff
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

int comp_pos(uint8_t *a, uint8_t*b) {
    return (a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]) || (a[3] != b[3]);
}

void init_position() {
    home_motor_enc(0);
    // goto_pos(0, 14);
    home_motor_enc(1);
    // goto_pos(1, 70);
    home_motor_enc(2);
    // goto_pos(2, 15);
    home_motor_enc(3);
    // goto_pos(3, 70);

    // reset_encoder(0);
    // reset_encoder(1);
    // reset_encoder(2);
    // reset_encoder(3);
}
