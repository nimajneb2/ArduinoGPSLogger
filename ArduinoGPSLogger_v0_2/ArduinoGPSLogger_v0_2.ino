// Libraries
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <SPI.h>
#include <SD.h>


// Variables
//Chip select for SD
const int chipSelect = 10;
File myFile;

int count = 0;
int myHour = 0;
int myMinute = 0;
int mySecond = 0;
int lastMinute = 0;
int timezonehour = 4;

//GPS
//static const int RXPin = 10, TXPin = 11; // Connect GT-U7 TXD to Arduino D10, GT-U7 RXD to Arduino D11
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600; // GT-U7 default baud rate
// Create a SoftwareSerial object for communication with the GPS
SoftwareSerial ss(RXPin, TXPin);
// Create a TinyGPS++ object
TinyGPSPlus gps;

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240}; // UTC-4
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};  // UTC-5
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr; // pointer to current rule

//months variable
const char* monthNames[] = {"January", "February", "March", "April", "May", "June",  "July", "August", "Sept.", "October", "November", "December" };
int monthNumber = 0;

// Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

void setup() {
  // Initialize serial communication
  u8g2.begin();
  Serial.begin(9600);
  while (!Serial) {
    delay(1); // Wait for serial port to connect (needed for Leonardo, Micro, etc.)
  }

  ss.begin(GPSBaud);    // Initialize serial communication with the GPS module
  Serial.println(F("GT-U7 GPS Module Test"));
  Serial.println(F("Waiting for GPS data..."));
  Serial.println();

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }
}


void loop() {
u8g2.clearBuffer();

//Read GPS via serial
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

// Set Time
setTime(gps.time.hour(),gps.time.minute(),gps.time.second(),gps.date.day(),gps.date.month(),gps.date.year());
     time_t utc = now();
    time_t local = myTZ.toLocal(utc, &tcr);
    //Serial.println();

//Timezone Variables
  char buf2[32];
  char buf[32];
  char m[4];
  
//Month Format as Oct
  strcpy(m, monthShortStr(month(local)));
//Format year month day
  sprintf(buf2, "%d/%02d/%02d", year(local), month(local), day(local));
//Format Hour Minute Second
  sprintf(buf, "%02d:%02d:%02d", hour(local), minute(local), second(local));

// Display time and date on OLED display
  u8g2.setFont(u8g2_font_gb24st_t_1);
  u8g2.setCursor(1, 32);
  u8g2.print(buf2);
  u8g2.setFont(u8g2_font_gb24st_t_1);
  u8g2.setCursor(1, 14);
  u8g2.print(buf);
  u8g2.setCursor(1, 32);
  
// Serial Print GPS Coords
  /*
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
  } else {
    Serial.println("GPS not updated");
  }
  */
  // If no GPS data is received after a certain time, indicate a potential issue
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: Check wiring and module power."));
  }

  // Print GPS Coords to OLED display
  u8g2.setCursor(0, 50);
  u8g2.print(gps.location.lat(), 6);
  u8g2.setCursor(0, 64);
  u8g2.print(gps.location.lng(), 6);
 //Send to display
  u8g2.sendBuffer();

//Serial.println("now to SD");
  //Serial.println(minute(local));
  //Serial.println(lastMinute);

// SD card variable (8 Character limit)
char filename[25];
sprintf(filename, "%04d%02d%02d.csv", year(local), month(local), day(local));
//Serial.println(filename);

//Write to SD card every minute
if (lastMinute != minute(local)) {
    lastMinute = minute(local);
    Serial.println("Writing time to SD");
    Serial.println("Date,      Time,    Latitude, Longitude,Altitude,Satellites,Speed(kmph)");
    Serial.print(buf2);
    Serial.print(",");
    Serial.print(buf);
    Serial.print(",");
    //Serial.print(F("Latitude: "));
    Serial.print(gps.location.lat(), 6); // Print latitude with 6 decimal places
    Serial.print(",");
    //Serial.print(F("Longitude: "));
    Serial.print(gps.location.lng(), 6); // Print longitude with 6 decimal places
    Serial.print(",");
    //Serial.print(F("Altitude: "));
    Serial.print(gps.altitude.meters());
    //Serial.print(F(" meters"));
    Serial.print(",");
    //Serial.print(F("Satellites: "));
    Serial.print(gps.satellites.value());
    Serial.print(",");
    //Serial.print(F("Speed: "));
    Serial.print(gps.speed.kmph());
    //Serial.print(F(" km/h"));
    Serial.print(",");
    Serial.println();
// Open file and write header
myFile = SD.open(filename, FILE_WRITE);
if (myFile) {
  if (myFile.size() == 0) {
    Serial.print("Writing to header...");
    myFile.println("Date,Time,Latitude,Longitude,Altitude,GPS Satellites Value,Speed(kmph)");
    Serial.println("done.");
  }
  // You can now write GPS data here
  myFile.close();
} else {
  Serial.print("error opening ");
  Serial.println(filename);
}

//Open file to write data
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to ");
    Serial.println(filename);
    
    myFile.print(buf2);
    myFile.print(",");
    myFile.print(buf);

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
    Serial.println("error opening");
    Serial.println(filename);
  }
}
  else
  {
    //serial print the minute is the same
    //Serial.println("Current minute is the same as the previous minute, not writing time to SD");
  }
  delay(250); // Wait for a second before getting the time again
}