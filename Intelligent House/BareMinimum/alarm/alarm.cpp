/*
 * alarm.cpp
 *
 * Created: 09/12/2021 12.30.20
 *  Author: jan
 */ 

#include "alarm.h"
#include "../common.h"

long delayAlarm = 0;										// Placeholder for timer1
extern bool AlarmOn;
extern bool ArmPerim;

#pragma region Alarm
void Alarm(int interval)
{
	if (!AlarmOn) return;
	bool sensPir = false;
	PrintLCD(0, 0, "     ARMED");
	digitalWrite(LED_ALARM, true);
	if ((millis() - delayAlarm) > interval)
	{
		delayAlarm = millis();
		if (!ArmPerim)
		{
			sensPir = Sensor_PIR();
		}
		bool sensMag = Sensor_Magnet();
		digitalWrite(LED_POLICE, false);
		if (sensPir || sensMag)
		{
			// TODO INTERRUPT
			digitalWrite(LED_POLICE, true);
			if (sensPir)		{ SerialLog("Motion detected!", "Motion sensor, living room", true); }
			else				{ SerialLog("Intrusion detected, door forced open!", "Door sensor, front door", true); }
		}
	}
}

bool Sensor_PIR()
{
	if (digitalRead(PIR))
	{
		return true;
	}
	return false;
}

bool Sensor_Magnet()
{
	if (digitalRead(REED_SWITCH))
	{
		return true;
	}
	return false;
}
#pragma endregion