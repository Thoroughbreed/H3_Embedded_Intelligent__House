#include "common.h"

#pragma region Variables
// EXTERNAL VARIABLES
extern bool ShowLog;
extern String lastDisarm;									// Last disarm time
extern String lastArm;										// Last arm time
extern String lastEvent;									// Last event time

// GLOBAL VARIABLES
bool AlarmOn = true;										// Is the system armed? (Starts with alarm ON)
bool PerimOn = false;										// Perimeter system armed?)
bool ArmSystem = false;										// Prepare to arm the system
bool ArmPerim = false;										// Only activates perimeter system
String climatePrint = "";									// Shows climate on OLED
#pragma endregion
#pragma region "Objects"
// Servos
Servo sWindow;
Servo sGarage;
Servo servos[] = { sWindow, sGarage };
// LCD
LiquidCrystal lcd(49, 47, 45, 43, 41, 39);
// Keypad incl. keypad pins
const char keys[COLS][ROWS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {37, 36, 35, 34};						// Pin numbering for rows in keypad
byte colPins[COLS] = {33, 32, 31, 30};						// Pin numbering for columns in keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// NFC reader
MFRC522 mfrc522(SS_PIN, RST_PIN);
// RTC
RTClib myClock;
// DHT
DHT dht(DHT_PIN, DHTTYPE);
#pragma endregion

#pragma region Initial setup
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