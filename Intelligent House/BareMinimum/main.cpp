#include "common.h"
// Servos
Servo sWindow;
Servo sGarage;
Servo servos[] = { sWindow, sGarage };
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

bool Hysterese(float val, float high, float low /* = 0 */)
{
	if (val < high && val > low)	{ return true;  }
	else							{ return false; }
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

#pragma region Climate

void Climate(int interval)
{
	if ((currentTime - delayClimate) > interval)
	{
		delayClimate = millis();
		PrintOLED(0,  30, Sensor_DHT(), 2);
		Sensor_MQ2();
	}
}

String Sensor_DHT()
{
	float t = dht.readTemperature();
	float h = dht.readHumidity();
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
	if (!Hysterese(h, 80))
	{
		if (!AlarmOn && servoWinPos < 180) { servoWinPos += 18; }
		else 
		{
			SerialLog(String("Humidity is over 80% (currently " + String(h) + "%)"), "Climate sensor, living room");
		}
	}
	else	{ servoWinPos = 0; }
		
	RunServo(WINDOW, servoWinPos);
	return String(String(t) + "C" + " " + String(h) + "rH");
}

void Sensor_MQ2()
{
	int ppm = analogRead(AIR);
	if (!Hysterese(ppm, 600))
	{
		if (!AlarmOn && servoGaragePos < 90) { servoGaragePos += 23; }
		else
		{
			SerialLog("Alert! Gas leak - Opening door!", "Air quality sensor, garage");
		}
	}
	else	{ servoGaragePos = 0; }
		
	RunServo(GARAGE, servoGaragePos);
}

#pragma endregion

#pragma region Entry and keypad

void Entry(int interval)
{
	if (!AlarmOn) return;
	String cardUid = "";
	if ((currentTime - delayEntry) > interval)
	{
		delayEntry = millis();
		PrintLCD(0, 1, "            ");
		cardUid = Sensor_Card();
		if (cardUid == "") return;
		
		SerialLog(cardUid, "Front door card reader");
		for (i = 0; i < 2; i++)
		{
			if (approvedCards[i] == cardUid)
			{
				NumAct = true;
				PrintLCD(0, 1, "Enter code:");
				SerialLog("APPROVED");
				return;
			}
			else
			{
				PrintLCD(0, 1, "  --DENIED!--");
				NumAct = false;
			}
		}
	}
}

String Sensor_Card()
{
	String result = "";
	if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() ) {
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
	
}

#pragma endregion






