# ArduinoGPSLogger
This simple Arduino code logs GPS coordinates from a GT-U7 GPS module to a file on an SD card. It also uses a RTC clock to show time. Displays both on an OLED display. 

## Arduino Libraries Used
\#include <Wire.h>\
\#include <RTClib.h>\
\#include <Arduino.h>\
\#include <SPI.h>\
\#include <U8g2lib.h>\
\#include <SD.h>\
\#include <SoftwareSerial.h>\
\#include <TinyGPS++.h>\

## Hardware used
Arduino Uno R4 Wifi\
Adafruit Logger\
0.96" 128x64 OLED (From Aliexpress)\
GT-U7 GPS Module (From Aliexpress)


## 0.96" 128x64 OLED via I2C

| OLED | Arduino |
|----------|----------|
| VCC  | 5VDC  |
| GND  | Ground  |
| SDA  | A4  |
| SCL  | A5  |

## GT-U7 Pinout

| GPS Module | Arduino |
|----------|----------|
| 1  | 5VDC  |
| 2  | Ground  |
| 3  | 4  |
| 4  | 3  |
| 5  | Not Connected  |





Here's a photo of my Arduino with the code running.\
<img height="400" alt="ArduinoGPSLogger photo" src="https://github.com/user-attachments/assets/3362d71e-a63f-4e82-8b4b-141422a48c32" />


Here's an image export from Fritzing.\
<img height="300" alt="ArduinoGPSLogger Fritzing" src="https://github.com/user-attachments/assets/94ac2901-d09e-4b4d-ad87-a9621f505acb" />







Things to do:

* ✅ Display time based on the GPS Module.
* ✅ Remove RTC code
* ✅ Fix power source.
* Research GPS functions available in Arduino library.
* Change GPS pins so it's 3-3 and 4-4

* Notes: After removing RTC, I'm not sure it's reliable in doors to rely on a $5 GPS module for time.
