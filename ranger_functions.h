#ifndef RANGER_FUNCTIONS_H
#define RANGER_FUNCTIONS_H

#include <stdint.h>
#include <stdbool.h>

//----RANGER VARIABLES----
extern volatile uint32_t echo_start;
extern volatile uint32_t echo_end;
extern volatile uint32_t last_distance;
extern volatile uint8_t echo_captured;
extern volatile bool checking_fall;

//----RANGER FUNCTIONS----
void Init_Ranger(void);
void Timer1B_Handler(void);

void Ranger_Trigger(void);
uint32_t Ranger_Get_Distance(void); //returns the last_distance in cm

//HELPER FUNCTIONS:
void Ranger_TRIG_High(void);
void Ranger_TRIG_Low(void);

#endif