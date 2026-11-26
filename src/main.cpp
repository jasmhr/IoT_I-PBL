// Internet of Things 1 (243-3E5-DW)
// PBL - Smart Study Room Station
// Made by James Hamdi & Desire Gonyora
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
#include <WiFi.h> // WiFi Library
#include <Wire.h> // I2C Library

// Pin Numbers
#define GREEN_LED 33 // RFID Check-In
#define RED_LED 4 // RFID Check-In
#define RFID_RST 5 // RFID Check-In
#define RFID_SS 15 // RFID Check-In
#define SPI_SCK 18 // RFID Check-In, Data Logging
#define SPI_MISO 19 // RFID Check-In, Data Logging
#define SPI_MOSI 23 // RFID Check-In, Data Logging
#define SD_CS 13 // Data Logging
#define FAN_LED  25
#define POT_PIN 34
#define OLED_CS 14
#define OLED_DC 27
#define OLED_RST 17
#define BMP_SDA 21
#define BMP_SCL 22
#define HELP_BUTTON 32

// Global Variables
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ------------------------------- RFID Check-In (Done by James, 1/3) -------------------------------
// [1] D. Saju, “Lab #9: MFRC522” 243-3E5-DW Internet of Things I, Dawson College, Fall 2026
// [2] https://docs.arduino.cc/built-in-examples/digital/BlinkWithoutDelay/ 
MFRC522 rfid(RFID_SS, RFID_RST); // Create MFRC22 Object
String lastUID = ""; // Store Last Scanned UID
unsigned long lastScanTime = 0;  // Store Last Scan Time
String authorizedUIDs[] = {"D0 A3 F5 5F"}; // Store Authorized UIDs
int numAuthorized = 1; // Number Of Authorized UIDs
bool roomOccupied = false; // Track Room Status
int ledState = LOW; // Variable to make the Red LED Flash
const long interval = 1000; // Blinking Interval in ms
unsigned long previousMillis = 0; // Store Last LED Update
unsigned long currentMillis = millis(); // Store Current Millis()

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
  if (roomOccupied == true) { // Runs if the room is occupied
    digitalWrite(GREEN_LED, HIGH); 
    // checkInCallback(); // Call checkIn handler
    onCheckIn();
    Serial.println("OCCUPIED"); 
  } 
  else { // Runs if the room is checked out of or free
    // checkOutCallback(); // Call checkOut handler
    onCheckOut();
    Serial.println("FREE");
  }
}

// onAccessDenied() is called when an invalid card is detected
void onAccessDenied() { 
  Serial.println("ACCESS DENIED"); 
  allIndicatorsOff(); 
  if (currentMillis - millis() >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  digitalWrite(RED_LED, ledState);
  }
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

// ------------------------------- Data Logging (Done by James, 2/3) -------------------------------
// [5] D. Saju, “Lab #5: SPI & SD Card” 243-3E5-DW Internet of Things I, Dawson College, Fall 2026
// [6] D. Saju, “Lab #10: Event-Driven Programming” 243-3E5-DW Internet of Things I, Dawson College, Fall 2026
unsigned long timeSeconds = millis() / 1000; // Track time
File dataFile; // Create file object

// volatile bool checkInFlag = false; // Flag a check-in
// volatile bool checkOutFlag = false; // Flag a check-out
// typedef void (*Callback)();
// Callback checkInCallback;
// Callback checkOutCallback;
// Callback helpButtonCallback;

void onCheckIn() {
  dataFile = SD.open("log.csv", FILE_WRITE);
  if(dataFile) {
    dataFile.print(timeSeconds);
    dataFile.print(",");
    dataFile.print("Check-In");
    dataFile.close();
    Serial.println("Check-In Logged");
  }
  else {
    Serial.println("Error Opening File");
  }
}

void onCheckOut() {
  dataFile = SD.open("log.csv", FILE_WRITE);
  if(dataFile) {
    dataFile.print(timeSeconds);
    dataFile.print(",");
    dataFile.print("Check-Out");
    dataFile.close();
    Serial.println("Check-Out Logged");
  }
  else {
    Serial.println("Error Opening File");
  }
}

void onHelpRequested() {
  dataFile = SD.open("log.csv", FILE_WRITE);
  if(dataFile) {
    dataFile.print(timeSeconds);
    dataFile.print(",");
    dataFile.print("Help Requested");
    dataFile.close();
    Serial.println("Help Request Logged");
  }
  else {
    Serial.println("Error Opening File");
  }
}

// Callbacks
// checkInCallback = onCheckIn;
// checkOutCallback = onCheckOut;
// helpButtonCallback = onHelpRequested;


// ------------------------------- Staff Data Page (Done by James, 3/3) -------------------------------
// [7] D. Saju, " Lab #7: ESP32 Wi-Fi Access Points and Web Servers" 243-3E5-DW Internet of Things I, Dawson College, Fall 2026
const char* ssid = "SmartStudyRoomJD"; // Wi-Fi Access Point SSID
const char* password = "12345678"; // Wi-Fi Access Point Password
WebServer server(80); // Create Web Server Object

// [SPECIAL FEATURE] Staff Message over BLE (Done by Desire & James, 1/1)

void setup() {
  // Data Logging Setup
  SPI.begin(); // Initialize SPI
  millis(); // Initialize millis() function to track time since start-up
  Serial.begin(115200);
  Serial.println("Program Started");
  Serial.println("Initializing SD Card...");
  if(SD.begin(SD_CS)) { 
    Serial.println("SD Card Found"); 
  } 
  else { 
    Serial.println("SD Card Not Found"); 
  }

  dataFile = SD.open("log.csv", FILE_WRITE);
  if(dataFile) {
    dataFile.println("Time, Event"); // CSV Header
    dataFile.close();
    Serial.println("CSV Header Written");
  }
  else {
    Serial.println("Failed To Create File");
  }

  // Wi-Fi code
  WiFi.softAP(ssid, password);
  Serial.print("IP Address: ");
  Serial.println( WiFi.softAPIP());
  // Server start
  server.begin();
  Serial.println("Web Server Started");

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
  // Server Loop
  server.handleClient();

  // RFID Check-In Loop
  if (rfid.PICC_IsNewCardPresent() == true && rfid.PICC_ReadCardSerial() == true) { 
    String uid = getUID(); 
    onCardScanned(uid); 
    rfid.PICC_HaltA(); 
    rfid.PCD_StopCrypto1();
  }
}