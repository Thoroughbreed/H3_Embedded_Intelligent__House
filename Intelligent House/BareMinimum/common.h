/*
 * common.h
 *
 * Created: 08/12/2021 12.25.06
 *  Author: jan
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>			// Core 
#include <Keypad.h>				// 4x4 keypad
#include <SPI.h>				// SPI comms
#include <Wire.h>				// I2C comms
#include <MFRC522.h>			// RFID reader

#include "alarm/alarm.h"		// Alarm functions
#include "climate/climate.h"	// Climate functions
#include "shared/shared.h"		// Shared/on-demand functions

// Misc defines
#define LED_RED 3
#define LED_GREEN 4
#define LED_BLUE 5
// RFID
#define RST_PIN 13
#define SS_PIN 53
// KEYPAD
#define ROWS 4												// Number of rows in keypad
#define COLS 4

// Initial functions
void Init_Displays();
void Init_Servo();
void Init_Sensors();
void Init_Comms();

// On-demand functions

// Loop functions
void Entry(int interval);									// Checks the entry point if alarm is active
void KeyIn();												// Controls the keypad input device

// Nested functions
String Sensor_Card();										// Checks for valid RFID card
void EnterPassword(char key);								// Enables possibility to enter password
bool CheckPassword();										// Checks the entered password
void Unlock();												// Disables alarm, unlocks etc.

#endif /* COMMON_H_ */