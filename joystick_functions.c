#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//----THIS IS THE JOYSTICK_FUNTCIONS FILE----


//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"

//----DEFINITIONS----

//----FUNCTIONS---- 
void Joystick_Init(void) //Utilising GPIO E Pin 3(PE3) - AIN0
{
	//GPIO Configuration:
	SYSCTL->RCGCGPIO |= (1 << 4); 
	while((SYSCTL->PRGPIO & (1 << 4)) == 0);
	
	GPIOE_AHB->AMSEL |= (1 <<3); //enable ANALOG
	GPIOE_AHB->DIR &= ~(1 << 3); 
	GPIOE_AHB->AFSEL |= (1 << 3); //Turning on alternative functions
	GPIOE_AHB->DEN &= ~(1 << 3); //Turning off DIGI - this uses AIN0
	
	//ADC Configuration:
	SYSCTL->RCGCADC  |= (1 << 0); //cause using ADC0
	while ((SYSCTL->PRADC & (1 << 0)) == 0);
	
	ADC0->PC = 0x1; //setting the sample rate
	ADC0->ACTSS &= ~(1 << 3); //disabling the sample sequencer (SS3)
	ADC0-> EMUX &= ~(0xF << 12);  //clearning emux
	ADC0->SSMUX3 = 0x0; //AIN0 to PE3
	ADC0->SSCTL3 = (1 << 1) | (1 << 2); //END0 = 1, IE0 = 1
	ADC0->ACTSS |= (1 << 3); //enabling it again
}

uint32_t Read_Joystick(void)
{
		ADC0->PSSI = (1 << 3);                   // Start conversion on SS3
    while((ADC0->RIS & (1 << 3)) == 0);      // Wait for conversion complete
    uint32_t result = ADC0->SSFIFO3 & 0xFFF; // Read 12-bit result
    ADC0->ISC = (1 << 3);                    // Clear completion flag
    return result;                           // Return 0–4095
}

void Joystick_Control_Servo(void)
{
	static uint32_t starting_servo_angle = 90; //making sure I'm starting the centre
	uint32_t value = Read_Joystick();
	//ES_Uprintf(0, "ADC: %lu\n", value); //LEFT = 0, RIGHT = 3.3V (4095)
	
	if(value < (2048 - 200))
	{
		//Moving the joystick is moving left
		if (starting_servo_angle > 0 + 5)
		{
			starting_servo_angle -= 5;
		}
		else {starting_servo_angle = 0; }
		
		Servo_SetAngle(starting_servo_angle);
		ES_Uprintf(0, "LEFT -> Angle: %ld\n", starting_servo_angle);
	}
	else if (value > (2048 + 200))
	{
		//Moving the joystick is moving right
		if (starting_servo_angle < 180 - 5)
		{
			starting_servo_angle += 5;
		}
		else {starting_servo_angle = 180; }
		
		Servo_SetAngle(starting_servo_angle);
		ES_Uprintf(0, "RIGHT -> Angle: %ld\n", starting_servo_angle);
	}
	else
	{ 
		//NOTHING - cause it's in the centre
	}
	ES_msDelay(120);
}

//SWTCHINHING FUCNTIONS:
void SW_Init(void) //So we using PB4
{
    SYSCTL->RCGCGPIO |= (1 << 1); 
    while ((SYSCTL->PRGPIO & (1 << 1)) == 0);

    GPIOB_AHB->DIR &= ~(1 << 4); //input
    GPIOB_AHB->DEN |=  (1 << 4); //digla mode
    GPIOB_AHB->PUR |=  (1 << 4); //pull up resistor 
}

bool SW_Pressed(void)
{
	uint16_t pressed = ((GPIOB_AHB->DATA & (1 << 4)) == 0);
	ES_msDelay(10); //for debouncing
	return pressed;
}
