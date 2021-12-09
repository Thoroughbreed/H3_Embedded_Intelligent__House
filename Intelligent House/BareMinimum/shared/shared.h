/*
 * shared.h
 *
 * Created: 09/12/2021 12.53.34
 *  Author: jan
 */ 


#ifndef SHARED_H_
#define SHARED_H_


#include <LiquidCrystal.h>		// LCD Display
#include <Adafruit_SSD1306.h>	// OLED Display
#include <DS3231.h>				// RTC
#include <Servo.h>				// Servo

#define SERVO_WINDOW 11
#define SERVO_GARAGE 12
#define WINDOW 0
#define GARAGE 1
// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

void PrintLCD(int place, int line, String text);			// Prints a text on the LCD from the given placement
void WriteLCD(int place, int line, byte icon);				// Prints an icon on the LCD at the given placement
void RunServo(int servo, int angle);						// Runs servo on the given angle
String AdjustZero(int val);									// Ads leading zero on less than 10 numbers
String GetTime();											// Gets time from RTC and parses it as HH:MM:SS
String GetDate();											// Gets date from RTC and parses it as DD/MM/YYYY
String GetTimestamp();										// Returns a string with YY/MM/DD HH:MM:SS
void PrintOLED(int x, int y, String text, int textSize = 1);// Prints text on the OLED at pixel placement (!!)
void SerialLog(String logEvent, String device,
						bool error = false);				// Writes important events through the serial port (115200 baud)
bool Hysterese(float val, float high, float low = 0);		// Returns true if value is good
void UpdateOLED(int interval);								// Loops around and clears the OLED and updates timestamp




#endif /* SHARED_H_ */