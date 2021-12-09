/*
 * climate.cpp
 *
 * Created: 09/12/2021 12.42.51
 *  Author: jan
 */ 

#include "climate.h"
#include "../common.h"

long delayClimate = 0;										// Placeholder for timer2
extern String climatePrint;
extern bool AlarmOn;
extern int servoWinPos;
extern int servoGaragePos;
extern DHT dht;
 

#pragma region Climate
void Climate(int interval)
{
	if ((millis() - delayClimate) > interval)
	{
		delayClimate = millis();
		climatePrint = Sensor_DHT();
		Sensor_MQ2();
	}
}

String Sensor_DHT()
{
	int t = dht.readTemperature();
	int h = dht.readHumidity();
	// Temp control
	if (!Hysterese(t, 30, 20))
	{
		if (!AlarmOn)
		{
			if (t < 20) { digitalWrite(LED_RED,  true); }
			else		{ digitalWrite(LED_BLUE, true); }
		}
		else
		{
			SerialLog(String("Temperature is outside boundary (currently " + String(t) + "C"), "Climate sensor, living room");
		}
	}
	else
	{
		digitalWrite(LED_RED, false);
		digitalWrite(LED_BLUE, false);
		digitalWrite(LED_GREEN, true);
	}
	
	// Humidity control
	if (!Hysterese(h, 65))
	{
		if (!AlarmOn && servoWinPos < 180) { servoWinPos += 18; }
		else
		{
			SerialLog(String("Humidity is over 65% (currently " + String(h) + "%)"), "Climate sensor, living room");
		}
	}
	else	{ servoWinPos = 0; }
	
	RunServo(WINDOW, servoWinPos);
	return String(String(t) + "C" + " " + String(h) + "%rH");
}

void Sensor_MQ2()
{
	int ppm = analogRead(AIR);
	SerialLog(String(ppm), "MQ-2");
	if (!Hysterese(ppm, 400))
	{
		if (!AlarmOn && servoGaragePos < 90) { servoGaragePos += 23; }
		else
		{
			SerialLog("Alert! Gas leak - Opening door!", "Air quality sensor, garage", true);
		}
	}
	else	{ servoGaragePos = 0; }
	
	RunServo(GARAGE, servoGaragePos);
}
#pragma endregion
