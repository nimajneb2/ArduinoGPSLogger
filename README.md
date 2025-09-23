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

Here's a photo of my Arduino with the code running.\
![ArduinoGPS2](https://github.com/user-attachments/assets/3bd4c2ce-a4c1-4959-9ef2-df455d25f22a)


Things to do:

* Display time based on the GPS Module.
* Remove RTC code
* Fix power source.
* Research GPS functions available in Arduino library.
