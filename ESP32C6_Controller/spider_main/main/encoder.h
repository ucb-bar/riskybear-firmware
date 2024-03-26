// encoder.h
#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();
void reset_encoder(int num);

void get_counts(int *enc_pos);

int get_count(int num);
int get_angle(int num);

#endif