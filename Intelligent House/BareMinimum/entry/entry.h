/*
 * entry.h
 *
 * Created: 09/12/2021 13.26.16
 *  Author: jan
 */ 


#ifndef ENTRY_H_
#define ENTRY_H_

#include <MFRC522.h>			// RFID reader
#include <Keypad.h>				// 4x4 keypad

// RFID
#define RST_PIN 13
#define SS_PIN 53
// KEYPAD
#define ROWS 4												// Number of rows in keypad
#define COLS 4

// FUNCTION PROTOTYPING
void Entry(int interval);									// Checks the entry point if alarm is active
void KeyIn();												// Controls the keypad input device
String Sensor_Card();										// Checks for valid RFID card
void EnterPassword(char key);								// Enables possibility to enter password
bool CheckPassword();										// Checks the entered password
void Unlock();												// Disables alarm, unlocks etc.

#endif /* ENTRY_H_ */