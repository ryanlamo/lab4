/* Author: C1C Ryan Lamo
 * Created: 23 October 2013
 * Last Edited: 23 October 2013
 * Description: This program will act as a device driver for the LCD.
 * Requires LCD.h and LCD.c to be included.
 */

#include <msp430.h> 
#include "LCD.h"
#include "buttons/button.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    
    char * selection;
    char * messageline1 = "Message?";
    char * messageline2 = "Press123";

    char * string ="ECE 382 is my favorite class";
    char * string2 ="last name ever, first name greatest ";
    char * string3 ="like a sprained ankle";
    char * string4 ="boy I aint nothing to play with";
    
    initializeSPI();
    initializeLCD();
    clearLCD();
    writemessage(messageline1, messageline2);

    configureP1PinAsButton(BIT1|BIT2|BIT4);
    char buttons[]={BIT1, BIT2, BIT4};
    char pressbutton = pollP1Buttons(buttons, 3);

    switch (pressbutton)
    {
    case BIT1:
    	selection = string2;
    	break;
    case BIT2:
    	selection = string3;
    	break;
    case BIT4:
    	selection = string4;
    	break;
    }

    scrollmessage(string, selection);

    
    while (1)
    {
    	
    }
	
	return 0;
}
