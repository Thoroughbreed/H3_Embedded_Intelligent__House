#include "common.h"
// Servos
Servo sWindow;
Servo sGarage;
// LCD
LiquidCrystal lcd(49, 47, 45, 43, 41, 39);
// Keypad incl. keypad pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// NFC reader
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key mfKey;
// RTC
RTClib myClock;
// DHT
DHT dht(DHT_PIN, DHTTYPE);


#pragma region Initial setup
void Init_Displays()
{
	lcd.createChar(7, custBackslash);
	lcd.createChar(6, custC);
	lcd.createChar(0, custSmile);
	lcd.createChar(1, custSad);
	lcd.begin(16, 2);
	// OLED
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void Init_Servo()
{
	sWindow.attach(SERVO_WINDOW);
	sGarage.attach(SERVO_GARAGE);
}

void Init_Sensors()
{
	dht.begin();
	mfrc522.PCD_Init();
	pinMode(PIR, INPUT);
	pinMode(REED_SWITCH, INPUT);
	pinMode(LED_ALARM, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(LED_POLICE, OUTPUT);
}

void Init_Comms()
{
	Serial.begin(115200);
	SPI.begin();
	Wire.begin();
}

void setup()
{
	Init_Comms();
	Init_Displays();
	Init_Sensors();
	Init_Servo();
}
#pragma endregion

void loop()
{
	Alarm(5000);
	Climate(10000);
	Entry(5000);
	KeyIn();
	display.clearDisplay();
	UpdateOLED(500);
}

#pragma region OnDemand functions
void SerialLog(String logEvent, String device)
{
	String msg = GetTimestamp();
	msg += " " + device + " " + logEvent;
	Serial.println(msg);
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

#pragma endregion

#pragma region Alarm
void Alarm(int interval)
{
	if (!AlarmOn) return;
	PrintLCD(5, 0, "ARMED");
	digitalWrite(LED_ALARM, true);
	if ((currentTime - delayAlarm) > interval)
	{
		delayAlarm = millis();
		bool sensPir = Sensor_PIR();
		bool sensMag = Sensor_Magnet();
		if (sensPir || sensMag)
		{
			// TODO INTERRUPT
			digitalWrite(LED_POLICE, true);
			if (sensPir)		{ SerialLog("Motion detected!", "Motion sensor, living room"); }
			else				{ SerialLog("Intrusion detected, door forced open!", "Door sensor, front door"); }
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
		return false;
	}
	return true;
}
#pragma endregion


