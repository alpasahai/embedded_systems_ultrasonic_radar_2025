#include <TM4C129.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//----THIS IS THE LCD_FUNTCIONS FILE----


//----HEADER FILES----
#include "ES.h"
#include "servo_motor_functions.h"
#include "ranger_functions.h"
#include "LCD_Display.h"
#include "lcd_functions.h"

//----DEFINITIONS----


//----FUNCTIONS---- 
void initSPI(void)  //we using SSI2
{
	//GPIO CONFIG
	//PD0=DC, PD1=MOSI, PD2=CS, PD3=CLK
	SYSCTL->RCGCGPIO |= (1<<3); //Using PORT D
	while ((SYSCTL->PRGPIO & (1 << 3)) == 0);
 
	GPIOD_AHB->DIR  |= (1 << 0); //outputting
	GPIOD_AHB->DEN  |= (1 << 0)|(1 << 1)|(1 << 2)|(1 << 3);
	GPIOD_AHB->AFSEL |= (1 << 1)|(1 << 2)|(1 << 3); //dealing with MOSI, FSS, CLK
	
	GPIOD_AHB->AFSEL &= ~(1 << 0);
	GPIOD_AHB->PCTL &= ~((0xF << 0)|(0xF << 4)|(0xF << 8)|(0xF << 12));
	GPIOD_AHB->PCTL |=  (0x0 << 0)|(0xF << 4)|(0xF << 8)|(0xF << 12);
	GPIOD_AHB->AMSEL &= ~((1 << 0)|(1 << 1)|(1 << 2)|(1 << 3));
 
	//SSI CONFIG
	SYSCTL->RCGCSSI |= (1 << 2); while(!(SYSCTL->PRSSI & (1<<2)));
 
	SSI2->CR1 = 0; //setting it to master (dealing MOSI not MISO)
	SSI2->ICR = 0x3; //clear ROR/RT
	SSI2->CPSR = 10; //prescaler
	SSI2->CR0  = (0 << 8) | (1<<7) | (1<<6) | 0x7; // SCR=5, SPO=1, SPH=1, 8-bit
	SSI2->CR1 |= (1 << 1); //enabling
 
	// idle levels
	GPIOD_AHB->DATA |= (1 << 0); //toggling the data from the library
}
 
//this is for transmitting the data - bascially this what the functiosn use
void spi_Transmit(uint8_t data) 
{
    while ((SSI2->SR & (1 << 1)) == 0) {} //TNF - transmit FIFO 
    SSI2->DR = data;
    while (SSI2->SR & (1 << 4)) {}
}

void Init_All_LCD(void)
{
	//Basically calling the functiosn from the LCD_Display file
	  initSPI();
    initLCD();
    setRotation(2); //orientation of the text
    fillScreen(ILI9341_BLACK);

    //Configuring text defaults
    setCharConfig(ILI9341_WHITE, 2, 1, ILI9341_BLACK, 2);
}

//Thiis is for showing the simple text:
void Show_Angle_Distance(int angle, int dist)
{
	char buf[32];

//BASIC AF Display
//	// Clear the region where text is drawn
//  fillRect(0, 30, 240, 60, ILI9341_BLACK);
//	
//	//Angle line
//	sprintf(buf, "Angle: %3d", angle);
//	moveCursor(10, 40);
//	drawString(buf, strlen(buf));
//	
//	//Distance line
//	sprintf(buf, "Distance: %3d cm", dist);
//	moveCursor(10, 70);
//	drawString(buf, strlen(buf));
	
//Pretty experimenting stuff:

	//Radar diemensuions
	int radar_top = 20;
	int radar_bottom = 180;
	int radar_height = radar_bottom - radar_top;
	
	int header_top = 190;
	int header_height = 50;
	fillRect(0, header_top, 320, header_height, ILI9341_MAROON);
	
	//Background
	fillRect(0, 0, 320, 240, ILI9341_WHITE); //width = 240, height = 320
	
//	//GRIDSSS - so the horizonatl and vertical
//	for (int y = radar_top; y <= radar_bottom; y += 30) 
//	{
//		fillRect(0, y, 320, 2, ILI9341_PINK);
//  }
//  for (int x = 0; x <= 320; x += 40)
//	{
//		fillRect(x, radar_top, 2, radar_height, ILI9341_DARKGREY);
//	}
	
	//Pretty header
  setCharConfig(ILI9341_MAROON, 2, 1, ILI9341_WHITE, 2);
  moveCursor(40, 210);
  drawString("~ULTRASONIC~", 12);
	
	//The texts
  setCharConfig(ILI9341_MAROON, 2, 1, ILI9341_WHITE, 2);
  moveCursor(35, 150);
  drawString("~RADAR ALIVE~", 13);
	
	setCharConfig(ILI9341_PINK, 2, 1, ILI9341_WHITE, 2);
  sprintf(buf, "ANGLE:%3d", angle);
  moveCursor(10, 40);
  drawString(buf, strlen(buf));
//	
//	//Colour changing distance cause why not
//	uint16_t distColor = ILI9341_MAROON;
//  if (dist < 10) distColor = ILI9341_RED;
//  else if (dist < 20) distColor = ILI9341_PINK;
//  else if (dist < 40) distColor = ILI9341_DARKGREY;

  setCharConfig(ILI9341_PINK, 2, 1, ILI9341_WHITE, 2);
  sprintf(buf, "DISTANCE:%3d cm", dist);
  moveCursor(10, 70); //x  adn y
  drawString(buf, strlen(buf));

}
