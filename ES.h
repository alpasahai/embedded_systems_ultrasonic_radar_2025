#ifndef ES_H
#define ES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// System Clock Functions

// Allowable Frequencies to set (MHz)  120, 96 ,80, 60, 48 ,40, 32, 30, 24, 20, 16, 12, 10

extern uint32_t ES_setSystemClk(int freq) ;  	// freq in MHz eg 120 is 120,000,000. Finds frequency closest to the requested value and sets the system clock
extern uint32_t ES_getSystemClk(void) ;			  // returns the system clock frequency in Hz
// for example ES_setSystemClk(16) sets the system clock to 16MHz

//
// -------------------------------------------------------------------
//

// serial functions
// uart is the uart number eg 0 is UART0
extern void ES_Serial(int uart, char line_format[]) ;  
extern char ES_getchar(int uart) ;
extern void ES_putchar(int uart, char c) ;
extern void ES_Uprintf(int uart,const char *formatstr, ...) ;

//
// -------------------------------------------------------------------
//
// delay functions
extern void ES_usDelay(uint32_t ui32Us) ; // microsecond delay
extern void ES_msDelay(uint32_t ui32Ms) ; // millisecond delay


#endif // ES_H
