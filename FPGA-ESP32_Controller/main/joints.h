// Joints.h
#ifndef JOINTS_h
#define JOINTS_h

void joints_init(void);
void reset_encoders(void);
void reset_encoder(int num);

void motor_set_en(int state);
void motor_set_state(int state);

void set_motor_presc(int num, int presc);

void set_motor_speed(int num, int speed);
void set_motor_pos(int num, int pos);

int get_encoder(int num);


#endif