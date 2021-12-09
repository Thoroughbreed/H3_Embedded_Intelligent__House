/*
 * shared.cpp
 *
 * Created: 09/12/2021 12.53.44
 *  Author: jan
 */ 

#include "shared.h"
#include "../common.h"

extern String lastEvent;
extern RTClib myClock;
extern bool AlarmOn;
extern Servo servos[];
extern LiquidCrystal lcd;
extern Adafruit_SSD1306 display;
extern String climatePrint;

int servoWinPos = 0;										// Initial position for Servo1
int servoGaragePos = 0;										// Initial position for Servo2
long delayOLED = 0;											// Placeholder for timer4
long delayLog = 0;											// Placeholder for timer5
String lastDisarm = "";
String lastArm = "";
String lastEvent = "";
bool ShowLog = false;										// Swaps to show log on OLED



#pragma region OnDemand functions
void SerialLog(String logEvent, String device, bool error /* = false */)
{
	String msg = GetTimestamp();
	msg += " " + device + " " + logEvent;
	Serial.println(msg);
	if (error) { lastEvent = msg; }
}

String AdjustZero(int val)
{
	String result = "0";
	if (val <= 9)
	{
		result += String(val);
		return result;
	}
	return String(val);
}

String GetDate()
{
	String result = "";
	result += AdjustZero(myClock.now().day()) + "/";
	result += AdjustZero(myClock.now().month()) + "/";
	result += String(myClock.now().year());
	return result;
}

String GetTime()
{
	String result = "";
	result += AdjustZero(myClock.now().hour()) + ":";
	result += AdjustZero(myClock.now().minute()) + ":";
	result += AdjustZero(myClock.now().second());
	return result;
}

String GetTimestamp()
{
	String result = "";
	result += String(myClock.now().year()) + "/";
	result += AdjustZero(myClock.now().month()) + "/";
	result += AdjustZero(myClock.now().day()) + " - ";
	result += GetTime();
	return result;
}

void RunServo(int servo, int angle)
{
	servos[servo].write(angle);
}

void PrintLCD(int place, int line, String text)
{
	lcd.setCursor(place, line);
	lcd.print(text);
}

void WriteLCD(int place, int line, byte icon)
{
	lcd.setCursor(place, line);
	lcd.write(byte(icon));
}

void PrintOLED(int x, int y, String text, int textSize)
{
	display.setTextSize(textSize);
	display.setTextColor(WHITE);
	display.setCursor(x, y);
	display.println(text);
}

void UpdateOLED(int interval)
{
	if ((millis() - delayOLED) > interval)
	{
		delayOLED = millis();
		display.clearDisplay();
		if (!ShowLog)
		{
			PrintOLED(0, 0, GetDate());
			PrintOLED(0, 8, GetTime(), 2);
			PrintOLED(0, 49, climatePrint, 2);
		}
		else if (ShowLog)
		{
			if (!AlarmOn)
			{
				PrintOLED(0, 0, "Disarmed:");
				PrintOLED(0, 8, lastDisarm);
			}
			else
			{
				PrintOLED(0, 0, "Armed:");
				PrintOLED(0, 8, lastArm);
			}
			PrintOLED(0, 18, "Event:");
			PrintOLED(0, 26, lastEvent);
		}
		display.display();
	}
	if ((millis() - delayLog) > 10000)
	{
		delayLog = millis();
		ShowLog = false;
	}
}

bool Hysterese(float val, float high, float low /* = 0 */)
{
	if (val < high && val > low)	{ return true;  }
	else							{ return false; }
}


#pragma endregion
