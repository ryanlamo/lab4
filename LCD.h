/*
 * LCD.h
 *
 *  Created on: Oct 23, 2013
 *      Author: C14Ryan.Lamo
 * Description: Header file used to implement interaction
 * 				with geek box LCD.
 */

#ifndef LCD_H_
#define LCD_H_

void initializeSPI();

void initializeLCD();

void clearLCD();

void delayMilli();

void delayMicro();

void movecursortolinetwo();

void movecursortolineone();

void writecharacter(char character);

void writethemessage(char * messagestring);

void scrollmessage(char * messagestring1, char * messagestring2);

#endif
