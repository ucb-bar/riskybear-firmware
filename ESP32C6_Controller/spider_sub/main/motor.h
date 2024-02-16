// motor.h
#ifndef MOTORS_h
#define MOTORS_h

void motor_init(void);
void update_positions(int8_t *pos);

void set_motor(int number, float speed);

void goto_pos(int num, int target);
void move_pos(int num, int8_t curr_pos);
void move_positions(int8_t *curr_poses);

void home_motor_sw(int num);
void home_motor_enc(int num);

#endif