// Internet of Things 1 (243-3E5-DW)
// PBL - Smart Study Room Station
// Made By James Hamdi & Desire Gonyora
// December 3rd 2026

// Libraries
#include <Adafruit_GFX.h> // Graphics Library
#include <Adafruit_BMP280.h> // Sensor Library
#include <Adafruit_Sensor.h> // Sensor Library
#include <Adafruit_SSD1306.h> // OLED Display Library
#include <Arduino.h> // Arduino Library
#include <ArduinoJson.h> // Json Library
#include <BLE_UART.h> // Bluetooth Library
#include <MFRC522.h> // RFID Library
#include <SD.h> // SD Card Library
#include <SPI.h> // SPI Library
#include <WebServer.h> // WebServer Library
#include <Wifi.h> // WiFi Library
#include <Wire.h> // I2C Library

// Pin Numbers
#define GREEN_LED 33
#define RED_LED 4
#define FAN_LED  25
#define POT_PIN 34
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define RFID_SS 5
#define RFID_RST 16
#define SD_CS 13
#define OLED_CS 14
#define OLED_DC 27
#define OLED_RST 17
#define BMP_SDA 21
#define BMP_SCL 22
#define HELP_BUTTON 32


// Global Variables
// Status Byte (lecture 2 slides)
const byte OCCUPIED_MASK = 0b00000001;
const byte HOT_MASK      = 0b00000010;
const byte HELP_MASK     = 0b00000100;
byte statusByte = 0;


// OLED Display 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,
                         OLED_DC, OLED_RST, OLED_CS);
// pwm fan control - Lab 12
#define FAN_CHANNEL 0
#define FAN_FREQ 5000
#define FAN_RESOLUTION 8

// SENSING AND AVERAGING VARIABLES Lab 6 & 11

const int NUM_SAMPLES = 10;
float temperatures[NUM_SAMPLES];
float averageTemperature = 0;

// Temperature index array & valid temperature readings
int tempIndex = 0;
int tempCount = 0;

// FAN CONTROL VARIABLES - Lab 12
float tempPoint = 24.0;
int fanLevel = 0;

// SENSOR AND DISPLAY INITIALIZATION FLAGS - Lab 6
bool sensorReady = false;
bool oledReady = false;
// 
volatile bool helpFlag = false;
unsigned long lastHelpTime = 0;
unsigned long lastTempTime = 0;
unsigned long lastDisplayTime = 0;
const unsigned long TEMP_INTERVAL = 2000;
const unsigned long DISPLAY_INTERVAL = 500;
const unsigned long HELP_DEBOUNCE = 250;






// RFID Check-In (Done by James, 1/3)




// Sensing & Display (Done by Desire, 1/3)
void deselectSPI() {
  digitalWrite(RFID_SS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(OLED_CS, HIGH);
}



// Fan Control - Lab 12 is used as reference (Done by Desire, 2/3)

void adjustFanSpeed() {
  int potValue = analogRead(POT_PIN);
  tempPoint = map(potValue, 0, 4095, 180, 300) / 10.0;
  
}



// Event and Status Byte (Done by Desire, 3/3)

void IRAM_ATTR helpISR(){
helpFlag = true;
}

void checkHelpButton(){
  if(helpFlag)
    helpFlag = false;
   interrupts();
  
   unsigned long now = millis();
   if (now - lastHelpTime < HELP_DEBOUNCE) return;
   if (digitalRead(HELP_BUTTON) == LOW)
   
     lastHelpTime = now;
     changeStatus(HELP_MASK, true);
     Serial.println("Help Button Pressed");
   }

// Data Logging (Done by James, 2/3)



// Staff Data Page (Done by James, 3/3)



// Staff Message over BLE (Done by Desire & James, 1/1)

void setup() {
  pinMode(OLED_CS, OUTPUT);
  pinMode(HELP_BUTTON, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
 
  //Lab 12 ADC & PWM settings
analogReadResolution(12);
analogSetAttenuation(ADC_11db);
ledcAttach(FAN_LED, FAN_FREQ, FAN_RESOLUTION);
ledcWrite(FAN_CHANNEL, 0);

SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
 Wire.begin(BMP_SDA, BMP_SCL);
  sensorReady = bmp.begin(0x76);
  if (!sensorReady) sensorReady = bmp.begin(0x77);
  if (!sensorReady) Serial.println("BMP280 not found");

  deselectSPI();
  oledReady = display.begin(SSD1306_SWITCHCAPVCC);
  if (!oledReady) Serial.println("OLED initialization failed");
  deselectSPI();
  attachInterrupt(digitalPinToInterrupt(HELP_BUTTON), helpISR, FALLING);
}


void loop() {

}