#ifndef SERVO_MOTOR_FUNCTIONS_H
#define SERVO_MOTOR_FUNCTIONS_H

#include <stdint.h>

//----SERVO_MOTOR VARIABLES----
extern volatile uint32_t current_angle;
extern volatile int direction;

//----SERVO_MOTOR FUNCTIONS----
void Servo_Init(void);
void Servo_SetAngle(uint32_t angle);
void Servo_TimerInit(void);
void Timer3A_Handler(void);

//Helper functions
void Servo_TimerStart(void);
void Servo_TimerStop(void);

#endif
