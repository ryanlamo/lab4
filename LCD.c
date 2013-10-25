/*
 * LCD.c
 *
 *  Created on: Oct 23, 2013
 *      Author: C14Ryan.Lamo
 */

#include "LCD.h"
#define RS 0x40

void initializeSPI()
{
	UCB0CTL1 |= UCSWRST;
	
	UCB0CTL0 |= UCMSB|UCMST|UCSYNC|UCCKPH;
	
	UCB0CTL1 |= UCSSEL1;
	
	UCB0STAT |= UCLISTEN;
	
	P1SEL |= BIT5;
	
	P1SEL2 |= BIT5;
	
	P1SEL |= BIT7;
	
	P1SEL2 |= BIT7;
	
	P1SEL |= BIT6;
	
	P1SEL2 |= BIT6;
	
	UCB0CTL1 &= ~UCSWRST|UCCKPL;
	
	P1DIR |= BIT3;
}

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


void clearLCD();
{
	writecommandbyte(1);
}

void movecursortolinetwo()
{
	writeCommandByte(0xA8);
}

void movecursortolineone()
{
	writeCommandByte(0x80);
	
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

void delayMilli()
{
	_delay_cycles(1809);
}

void delayMicro()
{
	_delay_cycles(45);
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

void Write_to_LCD_4(char bytewantsend)
{
	unsigned char sendbyte=bytewantsend;
	
	sendbyte &= 0x0F;
	
	sendbyte |= LCDCON;
	
	sendbyte &=0x7F;
	
	Spi_send();
	
	delayMicro();
	
	sendbyte |= 0x80;
	
	Spi_send();
	
	delayMicro();
	
	sendbyte &= 0x7f;
	
	Spi_send();
	
	delayMicro();
	
}

void set_ss_lo()
{
	P1OUT &= ~BIT3;
}

void set_ss_hi()
{
	P1OUT |= BIT3;
}


void Write_to_LCD_8(char bytewantsend)
{
	unsigned char sendbyte=bytewantsend;
	
	sendbyte &= 0xF0;
	
	sendbyte = sendbyte >>4;
	
	Write_to_LCD_4(sendbyte);
	
	sendbyte = bytewantsend;
	
	sendbyte &= 0xF0;
	
	Write_to_LCD_4(sendbyte);
}

void SPI_send(char bytewantsend)
{
	char readbyte;
	
	set_ss_lo();
	
	UCB0TXBUF = bytewantsend;
	
	while(!(UCB0RXIFG & IFG2))
	{
		
	}
	
	readbyte = UCB0RXBUF;
	
	set_ss_hi();
}





