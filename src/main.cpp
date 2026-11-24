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
#define RFID_RST 5
#define RFID_RST 16
#define SD_CS 13
#define OLED_CS 14
#define OLED_DC 27
#define OLED_RST 17
#define BMP_SDA 21
#define BMP_SCL 22
#define HELP_BUTTON 32


// Global Variables
// Status Byte
const byte OCCUPIED_MASK = (1 << 0);
const byte HOT_MASK = (1 << 1);
const byte HELP_MASK = (1 << 2);
byte statusByte = 0;


// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,
                         OLED_DC, OLED_RST, OLED_CS);
// pwm fan control
#define FAN_CHANNEL 0
#define FAN_FREQ 5000
#define FAN_RESOLUTION 8

float temperatures[10] = {0};
int tempIndex = 0;
int tempCount = 0;
float averageTemperature = 0;
float setPoint = 24.0;
int fanLevel = 0;
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



// Fan Control - Lab 12 is used as reference (Done by Desire, 2/3)

void adjustFanSpeed() {
  int potValue = analogRead(POT_PIN);
  setPoint = map(potValue, 0, 4095, 180, 300) / 10.0;
  
}



// Event and Status Byte (Done by Desire, 3/3)

Void IRAM_ATTR helpISR(){
helpFlag = true;
}

Void checkHelpButton(){
  if(helpFlag)
    helpFlag = false;
   interrupts();
  
   unsigned long now = milis();
   if (now - lastHelpTime < HELP_DEBOUNCE) return;
   if (digitalRead(HELP_BUTTON) == LOW)
   
     lastHelpTime = now;
     changeStatus(HELP_MASK, true);
     Srerial.println("Help Button Pressed");
   }

// Data Logging (Done by James, 2/3)



// Staff Data Page (Done by James, 3/3)



// Staff Message over BLE (Done by Desire & James, 1/1)

void setup() 


void loop() {

}