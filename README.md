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
Inland 1.3" 128x64 OLED V2.0\
GT-U7 GPS Module (From Aliexpress)

The display and GPS module are running off of an external 5 volt DC power supply.


## Inland 1.3" 128x64 OLED V2.0 Pinout via SPI

| OLED | Arduino |
|----------|----------|
| VCC  | 5VDC  |
| GND  | Ground  |
| CLK  | 13  |
| MOSI  | 11  |
| DC  | 9  |
| CS  | 6  |

## GT-U7 Pinout

| GPS Module | Arduino |
|----------|----------|
| 1  | 5VDC  |
| 2  | Ground  |
| 3  | 4  |
| 4  | 3  |
| 5  | Not Connected  |





Here's a photo of my Arduino with the code running.\
<img width="390" alt="Photo of Arduino project with GPS module and display showing time" src=https://github.com/user-attachments/assets/3bd4c2ce-a4c1-4959-9ef2-df455d25f22a />

Here's an image export from Fritzing.\
<img width="390" height="441" alt="Image exported from Fritzing" src="https://github.com/user-attachments/assets/321bef75-45e4-4d5e-a6b3-b7a0ce1c939e" />






Arduino Uno R4 Wifi\
Adafruit Logger\
Inland 1.3" 128x64 OLED V2.0\
GT-U7 GPS Module (From Aliexpress)

Things to do:

* Display time based on the GPS Module.
* Remove RTC code
* Fix power source.
* Research GPS functions available in Arduino library.
* Change GPS pins so it's 3-3 and 4-4
