#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//----THIS IS THE MAIN PROJECT FILE----

//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"
#include "LCD_Display.h"
#include "lcd_functions.h"
#include "buzzer_functions.h"
#include "joystick_functions.h"

//---------------------UART0 CONFIGURATION--------------------
void UART0_Init(void) { //Using UART0, Port A (PA0, PA1)
    SYSCTL->RCGCUART |= (1 << 0);   //enable UART0 clock
    SYSCTL->RCGCGPIO |= (1 << 0);   //enable Port A clock
    while((SYSCTL->PRUART & (1 << 0)) == 0); // wait until ready
    while((SYSCTL->PRGPIO & (1 << 0)) == 0);

    GPIOA_AHB->AFSEL |= (1 << 0) | (1 << 1); //alt functions
    GPIOA_AHB->PCTL  &= ~0xFF; //clearning
    GPIOA_AHB->PCTL  |= (1 << 0);
		GPIOA_AHB->PCTL |= (1 << 4);
    GPIOA_AHB->DEN  |= (1 << 0) | (1 << 1); //Digital mode

    UART0->CTL &= ~(1 << 0); // disable UART0
    UART0->IBRD = 32;  // integer baud divisor
    UART0->FBRD = 35;  // fractional part
    UART0->LCRH = (0x3 << 5); // 8-bit, no parity, 1 stop
    UART0->CC   = 0x0;  // use system clock
    UART0->CTL  = (1 << 0) | (1 << 8) | (1 << 9); //enabling UART, TXE, RXE
}

//-----------------------MAIN--------------------------------
int main(void)
{
	ES_setSystemClk(60) ;  
	ES_Serial(0,"115200, 8, N, 1"); 
	UART0_Init();
	
	//Intialising functions
	Servo_Init();
	Servo_TimerInit(); //FOR THE AUTOMATIC SWEEPING
	
	SW_Init();
	Joystick_Init();
	
	Init_Ranger();
	Init_Buzzer();
	Init_All_LCD();
	
	__enable_irq();
	//TESTING LCD - this is also to see if it's actually awake
	fillScreen(ILI9341_MAROON);
	ES_msDelay(1000);
	fillScreen(ILI9341_MAGENTA);
	ES_msDelay(1000);
	fillScreen(ILI9341_PINK);
	ES_msDelay(1000);
	fillScreen(ILI9341_WHITE);
	
	//TESTING THE JOYSTICK
	bool manual_mode = false; 
	
	//TESTING THE LCD
	uint32_t distance_test = 20; //FOR testing purposes
	uint32_t test_current_angle = 180;	
	
//---------------------------------------------------	
	while(1)
	{	
//		ES_Uprintf(0, "SHOULD RUN BABES\n");	
//		Show_Angle_Distance(test_current_angle, distance_test);
//		ES_Uprintf(0, "come on LCD - should have displayed\n");	
//		ES_msDelay(1000);
	
//---------------------------------------------------	
//TESTING SERVO AND THE JOYSTICK		
		if(SW_Pressed())
		{
			manual_mode = !manual_mode;  //if switch pressed, it's manual mode
			ES_Uprintf(0, "Mode changed: %s\n", manual_mode ? "MANUAL" : "AUTOMATIC");
			ES_msDelay(200);
		}
		if(manual_mode)
		{
			Servo_TimerStop();
			Joystick_Control_Servo();
			//ES_msDelay(80);
		}
		else
		{
			Servo_TimerStart();
			//the automatic sweep
		}

//---------------------------------------------------	
//TESTING THE PRINITNG AND THE RANGER
		Ranger_Trigger();
		ES_msDelay(60);
		
		if(echo_captured)
		{
			echo_captured = 0;
			uint32_t distance = Ranger_Get_Distance();
			
			//HERE YOU CAN PRINT YOUR ANGLE AND DISTANCE - on Teraterm to check: 
			ES_Uprintf(0, "Angle: %d, Distance: %d cm\n", current_angle, distance);
			
			//LCD Printing
			//to stop the glitching
			__disable_irq();
			Show_Angle_Distance(current_angle, distance);
			__enable_irq();
			
			//Buzzer Application
			if (distance <= 5) //Adding a threshold - when object is too close - 5cm within radius
			{   
        On_Buzzer();
			} 
      else 
      {
        Off_Buzzer();
			}
		} 
		else 
		{ 
			ES_Uprintf(0, "NOTHING\n"); 
		}
//-----------END OF WHILE LOOP-------------------------		
	} 
} 
		
