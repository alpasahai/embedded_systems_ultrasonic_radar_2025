#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//----THIS IS THE RANGER_FUNCTIONS FILE----


//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"

//----DEFINITIONS/NOTES----
/*
Ok so: The Trigger is gonna use Port B (PB2)
and the ECHO is using PORT A (PA3) - cause i need to use T1CCP1 (0x3)
*/

volatile uint32_t echo_start = 0;
volatile uint32_t echo_end = 0;
volatile uint32_t last_distance = 0;
volatile uint8_t echo_captured = 0;
volatile bool checking_fall = false;

//----FUNCTIONS---- 
void Init_Ranger(void)
{
	//GPIO Configuration for the TRIG AND ECHO
	SYSCTL->RCGCGPIO |= (1 << 0) | (1 << 1); //PORT A AND PORT B
	while ((SYSCTL->PRGPIO & ((1 << 0)|(1 << 1))) == 0);
	
	//TRIGGER STUFF - PORT B - PB2
	GPIOB_AHB->DIR |= (1 << 2); //Outputing 
	GPIOB_AHB->DEN |= (1 << 2); //Turning on DIGI
	GPIOB_AHB->AFSEL &= ~(1 << 2); //Alternative Functions off
	GPIOB_AHB->DATA &= ~(1 << 2); //LOW
	
	//ECHO STUFF - PORT A - PA3
	GPIOA_AHB->DIR &= ~(1 << 3); //Inputting
	GPIOA_AHB->DEN |= (1 << 3); //Turning on DIGI
	GPIOA_AHB->AFSEL |= (1 << 3); //Alternative Functions on
	GPIOA_AHB->PCTL  &= ~(0xF << 12); //Clearing 
	GPIOA_AHB->PCTL  |= (0x3 << 12); //T1CCP1
	GPIOA_AHB->PDR |= (1 << 3); //just a pull down

	//-----------------------------------------------------------------------//
	
	//USING TIMER1B for the edge-time capture for ECHO Pulse Width
	SYSCTL->RCGCTIMER |= (1 << 1); //Using Timer 1
	while((SYSCTL->PRTIMER & (1 << 1)) == 0);
	
	TIMER1->CTL &= ~(1 << 8); //disabling it to set it up
  TIMER1->CFG = 0x4; // change to 16 bit
	TIMER1->TBMR = 0; //clearing it
	TIMER1->TBMR |= 0x3; //setting it to cpature mode
	
	TIMER1->TBMR |= (1 << 2); //so this makes the edge-time mode
	TIMER1->TBMR |= (1 << 4); //and makes it count up
	
	//we want to cpature buth the rising (start) and the stop (falling)
	TIMER1->CTL &= ~(0x3 << 10); //clearing the TBEVENT
	TIMER1->CTL |= (0x3 << 10);  //this helps count both edges
	
  TIMER1->TBILR = 0xFFFF;  // lower 16 bits
  TIMER1->TBPR  = 0xFF;    // upper 8 bits (prescaler acts as high bits in 16-bit mode)
	
	//DOING THE INTERUPTS:
	TIMER1->ICR = (1 << 10); //clearing
	TIMER1->IMR |= (1 << 10); //enabling timer1b capture interupts
	
	NVIC_EnableIRQ(TIMER1B_IRQn);	
	NVIC_SetPriority(TIMER1B_IRQn, 2); //lowieng the priroity so it doesn't mess with servo

	//ENABLING THE TIMER
	TIMER1->CTL |= (1 << 8);
}

//-------------------------------------------------------------------//
void TIMER1B_Handler(void) //We are using Edge-time mode - Captures the exact timer value when an edge occurs
{
	
	  if (TIMER1->MIS & (1 << 10))  //capture event interrupt
    {
			uint32_t capture = (TIMER1->TBPR << 16) | TIMER1->TBR;
			
			if(!checking_fall)
			{
				//ES_Uprintf(0, "Rising edge\n");
				//its a rising edge
				echo_start = capture;
				checking_fall = true;
				
				//change to falling edge
        TIMER1->CTL &= ~(0x2 << 10);
        TIMER1->CTL |=  (0x2 << 10);
				//checking to see if the edge mode is changing
				uint32_t tbctl = TIMER1->CTL;
				//ES_Uprintf(0, "TBEVENT bits: %x\n", (tbctl >> 10) & 0x3);
			}
			else
			{
				//ES_Uprintf(0, "Falling edge\n");
				//its a falling edge
				echo_end = capture;
				checking_fall = false;
				
				//computing the width
				uint32_t pulse_width;
				if (echo_end >= echo_start) 
				{
					pulse_width = echo_end - echo_start; //echo end is the timer count when low, echo start is TC when HIGH
				}
				else {pulse_width = (0xFFFFFF - echo_start) + echo_end; } //ensuring the timer doesn't overflow

				// Timer is 60 MHz / 256 prescaler = 234.375 kHz -> 1 tick = 4.267 µs
				uint32_t time_us = (pulse_width * 4.267);  //microseconds
				
				//the distance in cm
				last_distance = time_us/58; //I could also do timeus/58
				
				echo_captured = 1; //flag for valid reading
				
				//ERROR HANDLING FOR DISTANCE:
				if (last_distance > 400) //cause 4m is it's max
					last_distance = 0;
				//ES_Uprintf(0, "Width: %1u us, Distance: %1u cm\n", time_us, last_distance);
				
				//switching back to the rising edge
				TIMER1->CTL &= ~(0x1 << 10);
				TIMER1->CTL |= (0x1 << 10);
			}
			
        //clearing the interupt flag
        TIMER1->ICR = (1 << 10);
				//ES_Uprintf(0, "ok we balling\n");
    }
}

//-------------------------------------------------------------------//
void Ranger_Trigger(void)
{
	Ranger_TRIG_High();
	ES_usDelay(100);
	Ranger_TRIG_Low();
}

uint32_t Ranger_Get_Distance(void)
{
	return last_distance; //this is measured cm value
}

//HELPER FUNCTIONS FOR TOGGLING TRIG:
void Ranger_TRIG_High(void)
{
	GPIOB_AHB->DATA |= (1 << 2);
}
	
void Ranger_TRIG_Low(void)
{
	GPIOB_AHB->DATA &= ~(1 << 2);
}
