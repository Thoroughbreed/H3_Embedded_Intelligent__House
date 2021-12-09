#include "common.h"

// LCD
LiquidCrystal lcd(49, 47, 45, 43, 41, 39);
// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// NFC reader
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key mfKey;
// RTC
RTClib myClock;
// DHT
DHT dht(DHT_PIN, DHTTYPE);
// KEYPAD
byte rowPins[ROWS] = {37, 36, 35, 34};						// Pin numbering for rows in keypad
byte colPins[COLS] = {33, 32, 31, 30};						// Pin numbering for columns in keypad
char keys[COLS][ROWS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};															// Keypad layout
Keypad keypad = Keypad(makeKeymap(keys), rowPins,
colPins, ROWS, COLS);		// Create keypad
String approvedCards[] = {"76bf341f", "04774d824d5380"};	// Approved UUID from RFID cards
const char pwd[4] = {'1', '3', '3', '7'};					// The "correct" password for the keypad
char pwdTest[4];											// Empty array for testing the PW
int pwdCount = 0;											// Counting number of chars in the PW test

// Servos
Servo sWindow;
Servo sGarage;
Servo servos[] = { sWindow, sGarage };
int servoWinPos = 0;										// Initial position for Servo1
int servoGaragePos = 0;										// Initial position for Servo2

long delayAlarm = 0;										// Placeholder for timer1
long delayClimate = 0;										// Placeholder for timer2
long delayEntry = 0;										// Placeholder for timer3
long delayOLED = 0;											// Placeholder for timer4
long delayLog = 0;											// Placeholder for timer5

String lastDisarm = "";										// Last disarm time
String lastArm = "";										// Last arm time
String lastEvent = "";										// Last event time
String climatePrint = "";									// Shows climate on OLED

bool AlarmOn = true;										// Is the system armed? (Starts with alarm ON)
bool PerimOn = false;										// Perimeter system armed?)
bool ArmSystem = false;										// Prepare to arm the system
bool ArmPerim = false;										// Only activates perimeter system
bool ShowLog = false;										// Swaps to show log on OLED
bool NumAct = false;

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
	RunServo(WINDOW, 0);
	RunServo(GARAGE, 0);
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
	UpdateOLED(500);
}

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
	bool sensPir = false;
	PrintLCD(0, 0, "     ARMED");
	// DEBUG
	//	PrintLCD(0, 1, "                ");
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
	// DEBUG
	Serial.println(ppm);
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

#pragma region Entry and keypad
void Entry(int interval)
{
	String cardUid = Sensor_Card();
	if ((millis() - delayEntry) > interval)
	{
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
				PrintLCD(0, 1, "Enter code:");
				SerialLog("APPROVED", "Front door card reader");
				NumAct = true;
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
				ShowLog	= true;
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
				if (NumAct)
				{
					EnterPassword(key);
				}
				else
				{
					pwdCount = 0;
				}
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

#pragma region OLED
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
#pragma endregion
