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


}
