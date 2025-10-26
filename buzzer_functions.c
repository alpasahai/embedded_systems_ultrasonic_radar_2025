#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//----THIS IS THE BUZZER_FUNTCIONS FILE----


//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"
#include "buzzer_functions.h"


//----DEFINITIONS----

//----FUNCTIONS---- 
void Init_Buzzer(void) // Uisng Port A Pin 4 (PA4)
{
	  SYSCTL->RCGCGPIO |= (1 << 0);  
    while((SYSCTL->PRGPIO & (1 << 0)) == 0);

    GPIOA_AHB->DIR |= (1 << 4);    //Output
    GPIOA_AHB->DEN |= (1 << 4);    //Digi enable
    GPIOA_AHB->DATA &= ~(1 << 4);  //Buzzer off
}

void On_Buzzer(void) {
	for (int i = 0; i < 2000; i++) //if i want to longer beep - increase 
	{
		GPIOA_AHB->DATA ^= (1 << 4);
		ES_usDelay(300); // helps change pitch - higher pitch meaning lower number
	}
}

void Off_Buzzer(void) {
    GPIOA_AHB->DATA &= ~(1 << 4);
}
