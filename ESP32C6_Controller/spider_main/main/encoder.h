// encoder.h
#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();
void reset_encoder(int num);

void get_counts(uint8_t *enc_pos);

int get_count(int num);
uint8_t get_angle(int num);

#endif