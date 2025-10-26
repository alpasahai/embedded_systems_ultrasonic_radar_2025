#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//----THIS IS THE SERVO_MOTOR_FUNTCIONS FILE----


//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"

//----DEFINITIONS----
volatile uint32_t current_angle = 0; 
volatile int direction = 1;

//----FUNCTIONS---- //ALPA FINDINGS: SO M0PWM2 uses PF2
void Servo_Init(void)
{
 //GPIO Configuration for the Servo - using Port F Pin 2(PF2) 
	SYSCTL->RCGCGPIO |= (1 << 5);
	while ((SYSCTL->PRGPIO & (1 << 5)) == 0);
	
	GPIOF_AHB->DIR |= (1 << 2); //Outputing for the Servo 
	GPIOF_AHB->DEN |= (1 << 2); //Turning on DIGI
	GPIOF_AHB->AFSEL |= (1 << 2); //Alternative Functions on
	GPIOF_AHB->PCTL  &= ~(0xF << 8); //Clearing 
	GPIOF_AHB->PCTL  |= (0x6 << 8); //M0PWM2
	
	
	//-----------------------------------------------------------------------//
	//PWM Setup
	//RECENT CHNAGES 2/10 - 2pm - so you need a divsior cause PWM can only handle
	//65535 counts - so change the CLK to 60Mhz and have the divisor as 64
	
	SYSCTL->RCGCPWM |= (1 << 0); //using M0PWM2
	while ((SYSCTL->PRPWM & (1 << 0)) == 0);
	
	PWM0->CC = (1 << 8) | 0x5; //Using a divisior of 64 <<<CHANGED THIS LINE -> NOW CHECK ON THE OSCILLISCPE
	PWM0->_1_CTL = 0; //Disbaling
	//0x2 = drive low on LOAD and 0x3 is drive HIGH on count down (CMPA)
	PWM0->_1_GENA = (0x2 << 2) | (0x3 << 6); //Refer to slide 11 of PWM peripheal
	
	PWM0->_1_LOAD = 18750 - 1; //20ms period
	PWM0->_1_CMPA = 18750 - 9375; //1.5ms pulse      //1520000; //~1ms pulse (0 degrees)
	
	//Enabling everythung
	PWM0->_1_CTL |= 1;
	PWM0->ENABLE |= (1 << 2);
}

void Servo_SetAngle(uint32_t angle) 
{
		if(angle > 180) angle = 180;
	
		//Pulse Width - 1ms (0) = 938 -> 2ms (180) = 1875 
		uint32_t pulse  = 938 + (angle * (1875-938))/180;
		PWM0->_1_CMPA = pulse; //18750 - pulse
}

void Servo_TimerInit(void)
{
    SYSCTL->RCGCTIMER |= (1 << 3); //Timer3
    while((SYSCTL->PRTIMER & (1 << 3)) == 0);

    TIMER3->CTL &= ~0x01;       //disable Timer3A
    TIMER3->CFG = 0x4;  //16bit
    TIMER3->TAMR = 0x02; //esnables the periodic mode
	
    TIMER3->TAILR = 65535; //20ms @ 60MHz
		TIMER3->TAPR = 55;
	
		TIMER3->ICR = 0x1;
    TIMER3->IMR |= 0x1;      //enable timeout interrupt
    
		NVIC_EnableIRQ(TIMER3A_IRQn); 
	
    TIMER3->CTL |= 0x01;        //enable Timer3A
}

void TIMER3A_Handler(void)
{
	  TIMER3->ICR = 0x1; //clearing interrupt
    //moving by a fixed step
    int next_angle = current_angle + (direction * 2);

    //reached or passed top limit
    if (next_angle > 180)
    {
        next_angle = 180;
        direction = -1;     //reverse for next step
        //ES_Uprintf(0, "UP: %d\n", next_angle);
    }
    //reached or passed bottom limit
    else if (next_angle < 0)
    {
        next_angle = 0;
        direction = 1;      //reverse for next step
        //ES_Uprintf(0, "DOWN: %d\n", next_angle);
    }

    current_angle = next_angle;

    Servo_SetAngle(current_angle);
		
		//Ranger_Trigger(); //this helps for the angle printing
}

//HELPER FUNCTIONS:
void Servo_TimerStart(void)
{
    TIMER3->CTL |= (1 << 0);  //enable Timer3A
}

void Servo_TimerStop(void)
{
    TIMER3->CTL &= ~(1 << 0); //disable Timer3A
}

