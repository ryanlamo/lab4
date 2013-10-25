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

void set_ss_hi()
{
	P1OUT |= BIT3;
}

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



void set_ss_lo()
{
	P1OUT &= ~BIT3;
}

void delayMilli()
{
	_delay_cycles(1809);
}

void delayMicro()
{
	_delay_cycles(45);
}

void delayTen()
{
	_delay_cycles(548005);
}

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

void writecommandnibble(char commandnibble)
{
	LCDCON &= ~RS;
	Write_to_LCD_4(commandnibble);
	delayMilli();
}

void writecommandbyte(char commandbyte)
{
	LCDCON &= ~RS;
	Write_to_LCD_8(commandbyte);
	delayMilli();
}

void writedatabyte(char databyte)
{
	LCDCON |= RS;
	Write_to_LCD_8(databyte);
	delayMilli();
}



void clearLCD()
{
	writecommandbyte(1);
}

void movecursortolinetwo()
{
	writecommandbyte(0xA8);
}

void movecursortolineone()
{
	writecommandbyte(0x80);

}

void writecharacter(char character)
{
	writedatabyte(character);
}

void writemessage(char * messagestring)
{
	char n=0;

	for (n=0; n<=8; n++)
	{
		writecharacter(messagestring[n]);
	}
}

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

/*
if (*(string1+8)==0x00)
	string1 = messagestring1;
if (*(string2+8)==0x00)
	string2 = messagestring2;
movecursortolineone();
for (i=0; i<8; i++)
	writecharacter(string1[i]);
movecursortolinetwo();
for (i=0; i<8; i++)
	writecharacter(string2[i]);

delayTen();
string1++;
string2++;
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



