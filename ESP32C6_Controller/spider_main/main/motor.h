// motor.h
#ifndef MOTORS_h
#define MOTORS_h

void motor_init(void);
void update_positions(int *pos);

void set_motor(int number, float speed);

void goto_pos(int num, int target);
void move_pos(int num, int curr_pos);
void move_positions(int *curr_poses);

void home_motor_sw(int num);

#endif