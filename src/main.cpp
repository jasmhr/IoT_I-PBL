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
#include <SPI.h> // SPI Library
#include <WebServer.h> // WebServer Library
#include <Wifi.h> // WiFi Library
#include <Wire.h> // I2C Library

// Pin Numbers
#define GREEN_LED 33 // RFID Check-In
#define RED_LED 4 // RFID Check-In
#define RFID_RST 5 // RFID Check-In
#define RFID_SS 15 // RFID Check-In
#define SPI_SCK 18 // RFID Check-In
#define SPI_MISO 19 // RFID Check-In
#define SPI_MOSI 23 // RFID Check-In
#define FAN_LED  25
#define POT_PIN 34
#define SD_CS 13
#define OLED_CS 14
#define OLED_DC 27
#define OLED_RST 17
#define BMP_SDA 21
#define BMP_SCL 22
#define HELP_BUTTON 32

// Global Variables
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ------------------------------- RFID Check-In (Done by James, 1/3) ------------------------------
// [1] D. Saju, “Lab #9: MFRC522” 243-3E5-DW Internet of Things I, Dawson College, Fall 2026
MFRC522 rfid(RFID_SS, RFID_RST); // Create MFRC22 Object
String lastUID = ""; // Store Last Scanned UID
unsigned long lastScanTime = 0;  // Store Last Scan Time
String authorizedUIDs[] = {"D0 A3 F5 5F"}; // Store Authorized UIDs
int numAuthorized = 1; // Number Of Authorized UIDs
bool roomOccupied = false; // Track Room Status

// getUID() turns the card's UID bytes into text
String getUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uid = uid + "0";
    }
    uid = uid + String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) {
      uid = uid + " ";
    }
  }
  uid.toUpperCase();
  return uid;
}

// isAuthorized() checks if the card's UID is in the authorizedUIDs array
bool isAuthorized(String uid) { 
  for (int i = 0; i < numAuthorized; i++) { 
    if (uid == authorizedUIDs[i]) { 
      return true; 
    } 
  } 
  return false; 
} 

// allIndicatorsOff() turns off all the LED indicators
void allIndicatorsOff() { 
  digitalWrite(GREEN_LED, LOW); 
  digitalWrite(RED_LED, LOW);
} 

// onAccessGranted() is called when a valid card is detected
void onAccessGranted() {
  allIndicatorsOff();
  roomOccupied = !roomOccupied;
  if (roomOccupied == true) {         // Runs if the room is occupied
    digitalWrite(GREEN_LED, HIGH); 
    Serial.println("OCCUPIED"); 
  } 
  else {                              // Runs if the room is free
    Serial.println("FREE");
  }
}

// onAccessDenied() is called when an invalid card is detected
void onAccessDenied() { 
  Serial.println("ACCESS DENIED"); 
  allIndicatorsOff(); 
  digitalWrite(RED_LED, HIGH);
}

// onCardScanned() is called when a card is scanned
void onCardScanned(String uid) { 
  if (uid == lastUID && millis() - lastScanTime < 2000) { 
    Serial.println("Repeat ignored"); 
    return; 
  } 
  lastUID = uid; 
  lastScanTime = millis(); 
  Serial.print("Card UID: "); 
  Serial.println(uid); 
    if (isAuthorized(uid) == true) { 
      onAccessGranted(); 
    } 
    else { 
      onAccessDenied(); 
  }
}

// Sensing & Display (Done by Desire, 1/3)

// Fan Control (Done by Desire, 2/3)

// Event and Status Byte (Done by Desire, 3/3)

// Data Logging (Done by James, 2/3)

// Staff Data Page (Done by James, 3/3)

// [SPECIAL FEATURE] Staff Message over BLE (Done by Desire & James, 1/1)

void setup() {
  Serial.begin(115200);
  Serial.println("Program Started");

  // Pin Setup
  pinMode(GREEN_LED, OUTPUT); // RFID Check-In
  pinMode(RED_LED, OUTPUT); // RFID Check-In

  // RFID Check-In Setup
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_DumpVersionToSerial();
  Serial.println("Tap a card...");
}

void loop() {
  // RFID Check-In Loop
  if (rfid.PICC_IsNewCardPresent() == true && rfid.PICC_ReadCardSerial() == true) { 
    String uid = getUID(); 
    onCardScanned(uid); 
    rfid.PICC_HaltA(); 
    rfid.PCD_StopCrypto1();
  }
}