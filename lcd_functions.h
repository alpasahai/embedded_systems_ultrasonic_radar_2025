#ifndef LCD_FUNCTIONS_H
#define LCD_FUNCTIONS_H

#include <stdint.h>

//----LCD VARIABLES----
extern volatile uint32_t current_angle;
extern volatile int direction;

//----LCD FUNCTIONS----
void initSPI(void);
void spi_Transmit(uint8_t dataIn);
void Init_All_LCD(void);
void Show_Angle_Distance(int c_angle, int dist);

//WE CAN ADD FUNCTIOSN FRO THE DISPAY LATER

#endif
