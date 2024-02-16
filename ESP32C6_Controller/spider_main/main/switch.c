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

#include "i2c.h"

int SW_REMAP[8] = {1, 0, 5, 4, 3, 2, 6, 7};
uint8_t vals;


void read_switches() {
    vals = i2c_read_io();
}

uint8_t read_switch(int num) {
    return (0x01) & (vals >> SW_REMAP[num]);
}

