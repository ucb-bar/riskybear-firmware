// encoder.h
#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();
void reset_encoder(int num);

void get_counts(int8_t *enc_pos);

int get_count(int num);
int8_t get_angle(int num);

#endif