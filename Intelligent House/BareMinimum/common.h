/*
 * common.h
 *
 * Created: 08/12/2021 12.25.06
 *  Author: jan
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>			// Core 
#include <SPI.h>				// SPI comms
#include <Wire.h>				// I2C comms

#include "alarm/alarm.h"		// Alarm functions
#include "climate/climate.h"	// Climate functions
#include "shared/shared.h"		// Shared/on-demand functions
#include "entry/entry.h"		// Entry functions

// Misc defines
#define LED_RED 3
#define LED_GREEN 4
#define LED_BLUE 5


// Initial functions
void Init_Sensors();
void Init_Comms();

#endif /* COMMON_H_ */