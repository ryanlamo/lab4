/* Author: C1C Ryan Lamo
 * Created: 23 October 2013
 * Last Edited: 23 October 2013
 * Description: This program will act as a device driver for the LCD.
 * Requires LCD.h and LCD.c to be included.
 */

#include <msp430.h> 
#include "LCD.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
	
	return 0;
}
