/*
 * LCD.c
 *
 *  Created on: Oct 23, 2013
 *      Author: C14Ryan.Lamo
 */
#include <msp430.h>
#include "LCD.h"
#define RS 0x40
char LCDCON = 0;

/*Function: set_ss_hi
 * Author: C1C Ryan Lamo
 * Description: Sets the slave select Hi
 */

void set_ss_hi()
{
	P1OUT |= BIT3;
}

/*Function: initializeSPI
 * Author: C1C Ryan Lamo
 * Description: Sets the pins on the chip to properly interact with
 * the Geek Box. Sets up pins for MISO and MOSI.
 */

void initializeSPI()
{
	P1DIR |= BIT3;

	set_ss_hi();

	UCB0CTL1 |= UCSWRST;
	
	UCB0CTL0 |= UCMSB|UCMST|UCSYNC|UCCKPH;
	
	UCB0CTL1 |= UCSSEL1;
	
	P1SEL |= BIT5;
	P1SEL2 |= BIT5;
	
	P1SEL |= BIT7;
	P1SEL2 |= BIT7;
	
	P1SEL |= BIT6;
	P1SEL2 |= BIT6;
	
	UCB0CTL1 &= ~UCSWRST;
}


/*Function: set_ss_lo
 * Author: C1C Ryan Lamo
 * Description: Sets the slave select Lo
 */
void set_ss_lo()
{
	P1OUT &= ~BIT3;
}

/*Function: delayMilli
 * Author: C1C Ryan Lamo
 * Description: Sets a delay of 1.65 milliseconds.
 */

void delayMilli()
{
	_delay_cycles(1809);
}

/*Function: delayMicro
 * Author: C1C Ryan Lamo
 * Description: Sets a delay of 45 microseconds.
 */
void delayMicro()
{
	_delay_cycles(45);
}

/*Function: delayTen
 * Author: C1C Ryan Lamo
 * Description: Sets a delay of 0.5 seconds.
 */
void delayTen()
{
	_delay_cycles(548005);
}

/*Function: SPI_send
 * Author: Capt Branchflower
 * Description: Enables chip to send bytes via SPI
 */

void SPI_send(char bytewantsend)
{
	volatile char readbyte;

	set_ss_lo();

	UCB0TXBUF = bytewantsend;

	while(!(UCB0RXIFG & IFG2))
	{

	}

	readbyte = UCB0RXBUF;

	set_ss_hi();
}

/*Function: Write_to_LCD_4
 * Author: C1C Ryan Lamo
 * Description: Writes a bit to the LCD
 */

void Write_to_LCD_4(char bytewantsend)
{
	unsigned char sendbyte=bytewantsend;
	
	sendbyte &= 0x0F;
	
	sendbyte |= LCDCON;
	
	sendbyte &=0x7F;
	
	SPI_send(sendbyte);
	
	delayMicro();
	
	sendbyte |= 0x80;
	
	SPI_send(sendbyte);
	
	delayMicro();
	
	sendbyte &= 0x7f;
	
	SPI_send(sendbyte);
	
	delayMicro();
	
}

/*Function: Write_to_LCD_8
 * Author: C1C Ryan Lamo
 * Description: Sends a byte to be printed to LCD.
 */

void Write_to_LCD_8(char bytewantsend)
{
	unsigned char sendbyte=bytewantsend;

	sendbyte &= 0xF0;

	sendbyte = sendbyte >>4;

	Write_to_LCD_4(sendbyte);

	sendbyte = bytewantsend;

	sendbyte &= 0x0F;

	Write_to_LCD_4(sendbyte);
}

/*Function: writecommandnibble
 * Author: Capt Branchflower
 * Description: Writes a command, in bit form, to the LCD
 */

void writecommandnibble(char commandnibble)
{
	LCDCON &= ~RS;
	Write_to_LCD_4(commandnibble);
	delayMilli();
}

/*Function: writecommandbyte
 * Author: Capt Branchflower
 * Description: Writes a command, in byte length, to the LCD
 */

void writecommandbyte(char commandbyte)
{
	LCDCON &= ~RS;
	Write_to_LCD_8(commandbyte);
	delayMilli();
}

/*Function: writedatabyte
 * Author: Capt Branchflower
 * Description: Writes data to the LCD, in byte length.
 */

void writedatabyte(char databyte)
{
	LCDCON |= RS;
	Write_to_LCD_8(databyte);
	delayMilli();
}

/*Function: clearLCD
 * Author: Capt Branchflower
 * Description: Clears the LCD.
 */

void clearLCD()
{
	writecommandbyte(1);
}

/*Function: movecursortolinetwo
 * Author: C1C Ryan Lamo
 * Description: Gives command to LCD to move cursor position to
 * second line
 */

void movecursortolinetwo()
{
	writecommandbyte(0xA8);
}

/*Function: movecursortolineone
 * Author: C1C Ryan Lamo
 * Description: Gives command to LCD to move cursor position to
 * first line
 */

void movecursortolineone()
{
	writecommandbyte(0x80);

}

/*Function: writecharacter
 * Author: C1C Ryan Lamo
 * Description: Takes a single character to be written to LCD
 */

void writecharacter(char character)
{
	writedatabyte(character);
}

/*Function: writemessage
 * Author: C1C Ryan Lamo
 * Description: Takes an entire message string to be written to LCD
 */

void writemessage(char * messagestring1, char * messagestring2)
{
	char n=0;
	movecursortolineone();
	for (n=0; n<=8; n++)
	{
		writecharacter(messagestring1[n]);
	}
	movecursortolinetwo();
	for (n=0; n<=8; n++)
		{
			writecharacter(messagestring2[n]);
		}
}

/*Function: printFromLocation
 * Author: C1C Ryan Lamo/Capt Branchflower
 * Description: Stores start location and current location of a message string
 * to allow for scrolling.
 */

char * printFromLocation(char * start, char * current)
{
	int i;

	if (*current == 0)
		current = start;

	char * displayChar = current;

	for (i = 0; i < 8; i++)
	{
		writecharacter(*displayChar);
		displayChar++;
		if (*displayChar == 0)
			displayChar = start;
	}

	return current + 1;
}

/*Function: scrollmessage
 * Author: C1C Ryan Lamo/Capt Branchflower
 * Description: Scrolls message across LCD for both line one and two.
 */

void scrollmessage(char *messagestring1, char * messagestring2)
{
	char i=0;
	char * current1 = messagestring1;
	char * current2 = messagestring2;

	while(1)
	{
		movecursortolineone();
		current1 = printFromLocation(messagestring1, current1);
		movecursortolinetwo();
		current2 = printFromLocation(messagestring2, current2);
		delayTen();
	}

}

/*Function: initializeLCD
 * Author: Capt Branchflower
 * Description: Initializes the LCD for use by the program.
 */


void initializeLCD()
{
	writecommandnibble(0x03);

	writecommandnibble(0x03);

	writecommandnibble(0x03);

	writecommandnibble(0x02);

	writecommandbyte(0x28);

	writecommandbyte(0x0c);

	writecommandbyte(0x01);

	writecommandbyte(0x06);

	writecommandbyte(0x01);

	writecommandbyte(0x02);

	SPI_send(0);
	delayMicro();
}



