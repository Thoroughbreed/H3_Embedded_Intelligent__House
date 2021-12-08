/*
 * lcd_icon.h
 *
 * Created: 08/12/2021 13.19.23
 *  Author: jan
 */ 


#ifndef LCD_ICON_H_
#define LCD_ICON_H_



byte custBackslash[8] = {
	0b00000,
	0b10000,
	0b01000,
	0b00100,
	0b00010,
	0b00001,
	0b00000,
	0b00000
};														// Backslash char
byte custC[8] = {
	0b00000,
	0b11010,
	0b11101,
	0b00100,
	0b00100,
	0b00101,
	0b00010,
	0b00000
};														// Centigrade char
byte custSmile[8] = {
	0b00000,
	0b00000,
	0b01010,
	0b00000,
	0b10001,
	0b01110,
	0b00000,
	0b00000
};														// Smiley char
byte custSad[8] = {
	0b00000,
	0b00000,
	0b01010,
	0b00000,
	0b01110,
	0b10001,
	0b00000,
	0b00000
};														// Smiley char



#endif /* LCD_ICON_H_ */