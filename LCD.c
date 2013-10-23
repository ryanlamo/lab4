/*
 * LCD.c
 *
 *  Created on: Oct 23, 2013
 *      Author: C14Ryan.Lamo
 */

#include "LCD.h"
#define RS 0x40

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
	
	while(!(UCB0RXIFG & IFGs))
	{
		
	}
	
	readbyte = UCB0RXBUF;
	
	set_ss_hi();
}





