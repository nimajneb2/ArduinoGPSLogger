#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
//i2c display setup 
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

int hour = 0;
int minute = 0;
int second = 0;
char filename;
int lastminute = 0;
//GPS
//static const int RXPin = 10, TXPin = 11; // Connect GT-U7 TXD to Arduino D10, GT-U7 RXD to Arduino D11
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600; // GT-U7 default baud rate
// Create a SoftwareSerial object for communication with the GPS
SoftwareSerial ss(RXPin, TXPin);
// Create a TinyGPS++ object
TinyGPSPlus gps;


//months variable
char *months[] = {"January", "February", "March", "April", "May", "June",  "July", "August", "Sept.", "October", "November", "December" };
int currentmonth = 0;

/*
const char* months[] = {
  "January", "February", "March", "April", "May", "June",  "July", "August", "September", "October", "November", "December";
}
*/
//Chip select for SD
const int chipSelect = 10;
File myFile;

RTC_PCF8523 rtc;
// delete SPI display U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 6, /* dc=*/ 9, /* reset=*/ 8);
//i2c display setup 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

void setup() {
  // Initialize serial communication
  u8g2.begin();
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // Wait for serial port to connect (needed for Leonardo, Micro, etc.)
  }

  // Try to initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Halt if RTC is not found
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }

myFile = SD.open("gps.csv", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to header...");
    myFile.println("Date,Time,Latitude,Longitude,Altitude,GPS Satellites Value,Speed(kmph)");
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }



  Serial.println("initialization done.");

  ss.begin(GPSBaud);    // Initialize serial communication with the GPS module
  Serial.println(F("GT-U7 GPS Module Test"));
  Serial.println(F("Waiting for GPS data..."));
  Serial.println();
}

void loop() {
//GPS Start and serial communication of location
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  if (gps.location.isUpdated()) {
    Serial.print(F("Latitude: "));
    Serial.println(gps.location.lat(), 6); // Print latitude with 6 decimal places

    Serial.print(F("Longitude: "));
    Serial.println(gps.location.lng(), 6); // Print longitude with 6 decimal places

    Serial.print(F("Altitude: "));
    Serial.print(gps.altitude.meters());
    Serial.println(F(" meters"));

    Serial.print(F("Satellites: "));
    Serial.println(gps.satellites.value());

    Serial.print(F("Speed: "));
    Serial.print(gps.speed.kmph());
    Serial.println(F(" km/h"));

    Serial.println();
  }
  // If no GPS data is received after a certain time, indicate a potential issue
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: Check wiring and module power."));
  }


  u8g2.clearBuffer();
  // Get the current date and time from the RTC
  DateTime now = rtc.now();

  second = now.second();
  minute = now.minute();
  hour = now.hour();

  currentmonth = now.month() - 1;
  
// Display time on display
//set font
  u8g2.setFont(u8g2_font_samim_16_t_all);
//Set cursor location
  u8g2.setCursor(0, 16);

// Hours
  if (hour < 10) {
    u8g2.print("0");
    u8g2.print(now.hour());
  }
  else {
    u8g2.print(now.hour());
  }
    u8g2.print(":");

// Minutes
  if (minute < 10) {
    u8g2.print("0");
    u8g2.print(now.minute());
  }
  else {
    u8g2.print(now.minute());
  }

  u8g2.print(":");

// Seconds
  if (second < 10) {
    u8g2.print("0");
    u8g2.print(now.second());
  }
  else {
    u8g2.print(now.second());
  }
  u8g2.setCursor(0, 34);
  //u8g2.print("Date: ");
    u8g2.print(months[currentmonth]);
    u8g2.print(" ");
    u8g2.print(now.day());
    u8g2.print(", ");
    u8g2.print(now.year());
//Serial.print(now.day());
//Serial.print(now.year());
  u8g2.setCursor(0, 50);
  u8g2.print(gps.location.lat(), 6);
  u8g2.setCursor(0, 64);
  u8g2.print(gps.location.lng(), 6);
 //Send to display
  u8g2.sendBuffer();

//save time to file

//Serial.println("now to SD");

if (lastminute != now.minute()) {
    lastminute = now.minute();
    Serial.println("Writing time to SD");

// Open file
  myFile = SD.open("gps.csv", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to gps.csv...");
    
    myFile.print(now.year());
    myFile.print('/');
    myFile.print(now.month());
    myFile.print('/');
    myFile.print(now.day());

    myFile.print(",");

    if (hour < 10) {
    myFile.print("0");
    myFile.print(now.hour());
    }
    else {
      myFile.print(now.hour());
    }
    myFile.print(":");

// Minutes
    if (minute < 10) {
      myFile.print("0");
      myFile.print(now.minute());
    }
    else {
      myFile.print(now.minute());
    }
    myFile.print(":");

// Seconds
    if (second < 10) {
      myFile.print("0");
      myFile.print(now.second());
    }
  
    else {
      myFile.print(now.second());
    }
    //myFile.print(",");

    //if (gps.location.isUpdated()) {
      myFile.print(",");
      myFile.print(gps.location.lat(), 6); // Print latitude with 6 decimal places
      myFile.print(",");
      myFile.print(gps.location.lng(), 6); // Print longitude with 6 decimal places
      myFile.print(",");
      myFile.print(gps.altitude.meters());
      myFile.print(",");
      myFile.print(gps.satellites.value());
      myFile.print(",");
      myFile.print(gps.speed.kmph());
      
 
  //}
     myFile.println();

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening gps.csv");
  }
  
  }
  else
  {
    //Serial.println("Current minute is the same as the previous minute, not writing time to SD");
  }

  delay(100); // Wait for a second before getting the time again
}
