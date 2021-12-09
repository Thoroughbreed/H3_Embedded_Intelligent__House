/*
 * climate.h
 *
 * Created: 09/12/2021 12.43.20
 *  Author: jan
 */ 


#ifndef CLIMATE_H_
#define CLIMATE_H_

#include <DHT.h>				// Temp/hum sensor

// DHT
#define DHT_PIN 23
#define DHTTYPE DHT11
// MQ-2 (Air quality)
#define AIR A0

void Climate(int interval);									// Monitors the in-house climate
String Sensor_DHT();										// Checks temperature and humidity
void Sensor_MQ2();											// Checks for air quality



#endif /* CLIMATE_H_ */