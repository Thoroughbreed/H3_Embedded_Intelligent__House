/*
 * alarm.h
 *
 * Created: 09/12/2021 12.30.08
 *  Author: jan
 */ 


#ifndef ALARM_H_
#define ALARM_H_

// Debug/proof of concept LED
#define LED_ALARM 2
#define LED_POLICE 6
// PIR (Motion sensor)
#define PIR 8
// REED (Magnetic sensor)
#define REED_SWITCH 10

void Alarm(int interval);									// Controls if the alarm system is active
bool Sensor_PIR();											// Checks for motion when alarm is active
bool Sensor_Magnet();										// Checks all entry points



#endif /* ALARM_H_ */