#ifndef JOYSTICK_FUNCTIONS_H
#define JOYSTICK_FUNCTIONS_H

#include <stdint.h>
#include <stdbool.h>

//----JOYSTICK VARIABLES----
//THE TYPE OF JOYSTICK IS KY-023

//----JOYSTICK FUNCTIONS----
void Joystick_Init(void);
uint32_t Read_Joystick(void);
void Joystick_Control_Servo(void);

//SWTICH CONTROL - to help turn manual mode on:
void SW_Init(void);
bool SW_Pressed(void);

#endif
