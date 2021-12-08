/*
 * common.h
 *
 * Created: 08/12/2021 12.25.06
 *  Author: jan
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>			// Core 
#include <LiquidCrystal.h>		// LCD Display
#include <Adafruit_SSD1306.h>	// OLED Display
#include <Keypad.h>				// 4x4 keypad
#include <Servo.h>				// Servo
#include <SPI.h>				// SPI comms
#include <Wire.h>				// I2C comms
#include <MFRC522.h>			// RFID reader
#include <DS3231.h>				// RTC
#include <DHT.h>				// Temp/hum sensor
#include "lcd_icon.h"			// Custom LCD icons
// #include <Adafruit_GFX.h>	  // DEPRECATED

// Misc defines
#define LED_ALARM 2
#define LED_RED 3
#define LED_GREEN 4
#define LED_BLUE 5
#define LED_POLICE 6
#define REED_SWITCH 10
#define SERVO_WINDOW 11
#define SERVO_GARAGE 12
#define WINDOW 0
#define GARAGE 1
// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
// MQ-2 (Air quality)
#define AIR A0
// PIR (Motion sensor)
#define PIR 8
// RFID
#define RST_PIN 13
#define SS_PIN 53
// DHT
#define DHT_PIN 23
#define DHTTYPE DHT11
// KEYPAD
#define ROWS 4												// Number of rows in keypad
#define COLS 4

// KEYPAD
byte rowPins[ROWS] = {37, 36, 35, 34};						// Pin numbering for rows in keypad
byte colPins[COLS] = {33, 32, 31, 30};						// Pin numbering for columns in keypad
char keys[COLS][ROWS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};															// Keypad layout
String approvedCards[] = {"76bf341f", "04774d824d5380"};	// Approved UUID from RFID cards
const char pwd[4] = {'1', '3', '3', '7'};					// The "correct" password for the keypad
char pwdTest[4];											// Empty array for testing the PW

// int pwdCount = 0;											// Counting number of chars in the PW test
int i = 0;													// For i
int servoWinPos = 0;										// Initial position for Servo1
int servoGaragePos = 0;										// Initial position for Servo2

long delayAlarm = 0;										// Placeholder for timer1
long delayClimate = 0;										// Placeholder for timer2
long delayEntry = 0;										// Placeholder for timer3
long delayOLED = 0;											// Placeholder for timer4
long currentTime;											// Current time

bool locked;												// Is the door locked?
bool AlarmOn;												// Is the system armed?
bool NumAct;												// Is the numpad active?

// Initial functions
void Init_Displays();
void Init_Servo();
void Init_Sensors();
void Init_Comms();

// On-demand functions
void PrintLCD(int place, int line, String text);			// Prints a text on the LCD from the given placement
void WriteLCD(int place, int line, byte icon);				// Prints an icon on the LCD at the given placement
void RunServo(int servo, int angle);						// Runs servo on the given angle
String AdjustZero(int val);									// Ads leading zero on less than 10 numbers
String GetTime();											// Gets time from RTC and parses it as HH:MM:SS
String GetDate();											// Gets date from RTC and parses it as DD/MM/YYYY
String GetTimestamp();										// Returns a string with YY/MM/DD HH:MM:SS
void PrintOLED(int x, int y, String text, int textSize = 1);// Prints text on the OLED at pixel placement (!!)
void SerialLog(String logEvent, String device);				// Writes important events through the serial port (115200 baud)
bool Hysterese(float val, float high, float low = 0);		// Returns true if value is good

// Loop functions
void Alarm(int interval);									// Controls if the alarm system is active
void Climate(int interval);									// Monitors the in-house climate
void Entry(int interval);									// Checks the entry point if alarm is active
void KeyIn();												// Controls the keypad input device
void UpdateOLED(int interval);								// Loops around and clears the OLED and updates timestamp

// Nested functions
bool Sensor_PIR();											// Checks for motion when alarm is active
bool Sensor_Magnet();										// Checks all entry points
String Sensor_DHT();										// Checks temperature and humidity
void Sensor_MQ2();										// Checks for air quality
void Sensor_Card();											// Checks for valid RFID card
void EnterPassword();										// Enables possibility to enter password
void CheckPassword();										// Checks the entered password

#endif /* COMMON_H_ */