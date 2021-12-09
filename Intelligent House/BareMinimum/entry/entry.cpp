/*
 * entry.cpp
 *
 * Created: 09/12/2021 13.26.07
 *  Author: jan
 */ 

#include "entry.h"
#include "../common.h"

String approvedCards[] = {"76bf341f", "04774d824d5380"};	// Approved UUID from RFID cards
const char pwd[4] = {'1', '3', '3', '7'};					// The "correct" password for the keypad
char pwdTest[4];											// Empty array for testing the PW
int pwdCount = 0;											// Counting number of chars in the PW test
long delayEntry = 0;										// Placeholder for timer3
bool NumAct = false;										// Is the numpad active?
MFRC522::MIFARE_Key mfKey;

extern MFRC522 mfrc522;
extern Keypad keypad;
extern String lastDisarm;// = "";										// Last disarm time
extern String lastArm;// = "";										// Last arm time
extern String lastEvent;// = "";										// Last event time
extern bool ShowLog;
extern bool ArmSystem;
extern bool ArmPerim;
extern bool AlarmOn;
extern bool PerimOn;

#pragma region Entry and keypad
void Entry(int interval)
{
	if ((millis() - delayEntry) > interval)
	{
		String cardUid = Sensor_Card();
		delayEntry = millis();
		PrintLCD(0, 1, "                ");
		ArmSystem = false;
		ArmPerim = false;
		if (cardUid == "") return;
		if (!AlarmOn) return;
		
		SerialLog(cardUid, "Front door card reader");
		for (int i = 0; i < 2; i++)
		{
			if (approvedCards[i] == cardUid)
			{
				NumAct = true;
				PrintLCD(0, 1, "Enter code:");
				SerialLog("APPROVED", "Front door card reader");
				return;
			}
			else
			{
				PrintLCD(0, 1, "   --DENIED--   ");
				SerialLog("DENIED", "Front door card reader");
				NumAct = false;
			}
		}
	}
}

String Sensor_Card()
{
	String result = "";
	if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
	{
		delay(50);
		return result;
	}
	
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		result = result + String(mfrc522.uid.uidByte[i], HEX);
	}
	return result;
}

void KeyIn()
{
	char key = keypad.getKey();
	if (key != NO_KEY)
	{
		switch (key)
		{
			case 'A':
			PrintLCD(0, 1, "Arm system?");
			ArmSystem = true;
			break;
			case 'B':
			PrintLCD(0, 1, "Arm perim?");
			ArmPerim = true;
			break;
			case 'C':
			ShowLog = true;
			break;
			case 'D':
			// Delete
			break;
			case '*':
			pwdCount = 0;
			if (CheckPassword())
			{
				Unlock();
			}
			else
			{
				PrintLCD(0, 1, "  --DENIED!--");
				SerialLog("Wrong pin", "Front door keypad", true);
			}
			break;
			case '#':
			if(ArmSystem)
			{
				AlarmOn = true;
				PrintLCD(0, 1, "                ");
				SerialLog("SYSTEM ARMED", "Front door keypad");
				lastArm = GetTimestamp();
			}
			if (ArmPerim)
			{
				AlarmOn = true;
				PerimOn = true;
				PrintLCD(0, 1, "                ");
				SerialLog("PERIMETER ARMED", "Front door keypad");
				lastArm = GetTimestamp();
			}
			break;
			default:
			if (!NumAct)
			{
				pwdCount = 0;
				break;
			}
			EnterPassword(key);
			break;
		}
	}
}

void Unlock()
{
	PrintLCD(0, 0, "                ");
	PrintLCD(0, 1, "                ");
	PrintLCD(0, 0, "Welcome");
	WriteLCD(8, 0, 0);
	AlarmOn = false;
	digitalWrite(LED_ALARM, false);
	digitalWrite(LED_POLICE, false);
	SerialLog("System disarmed", "Front door keypad");
	lastDisarm = GetTimestamp();
}

bool CheckPassword()
{
	// Logic testing of the password
	if (pwdTest[0] == pwd[0] && pwdTest[1] == pwd[1] && pwdTest[2] == pwd[2] && pwdTest[3] == pwd[3])
	{
		pwdTest[0] = 0;
		return true;
	}
	return false;
}

void EnterPassword(char key)
{
	if (pwdCount <= 3)
	{
		pwdTest[pwdCount] = key;
		PrintLCD(12, 1, "    ");
		for (int i = 0; i <= pwdCount; i++)
		{
			PrintLCD(i + 12, 1, "*");
		}
		pwdCount++;
	}
	else
	{
		pwdCount = 0;
		PrintLCD(12, 1, "    ");
	}
}
#pragma endregion