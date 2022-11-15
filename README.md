# NEO-6M-GPS-TIME
NEO-6M-GPS-CLOCK-DATE-LAT-LONG-SPEED-ALTITUDE-SAT USED

- Tested on chinese SMD ATMEL UNO later the codes grew and only worked on MEGA2560
- Added Uno expansion board for extra headers wiring reasons
- 3v3 to Neo VCC,5V to encoder.
- GND Neo to any common UNO/MEGA GND.
- NEO can only receive data ,NEO TX to UNO/MEGA Software Serial RX.So only 3 wire used.(VCC,GND,TX)
- Download any nessecary libraries for module example:TFT,NMEA,Adafruit_GFX...etc
- Feel free to try on other display..Led Matrix,serial monitor..etc
- Standard 24h UTC GPS time broadcast are local coded to 12h AM/PM format.
- Later added Rotary Encoder to cycle 40 timezone.
- Later added AUTO DECODE Timezone/DST by valid coordinate on lo-resolution squared Mapping , Rotary cycle if slow.
- Once a valid coordinate decode timezone ,Timezone are memorised and runs without a need for coordinate validation until a new    Timezone are found.
- Check diagram supplied in repo for brief idea.

LIST OF SKETCH IN THIS REPO
----------------------------
- Simple Pre-defined single Timezone for LOCAL GPS .
- Manual Rotary Cycled 40 Timezone ( UTC/DST)
- Auto Timezone/DST Decoder by valid coordinate (lo-res).
- FUTURE Hi-rsolution decoding includes colors recognition on bmp file(World Timezone Map) in SD card by valid coordinate.(not sure how yet)
- Tested espnow.h for esp8266 for wireless,router less,non-internet based Stratum 1 to 2 Gps Clock broadcast to multiple receivers Ledmatrix display (sync redundancy check).A single data processing transmitter could support at least 2 device in a close b/g/n range.Max is >5 device.. 

TIMEZONE LIST
-----------------
https://en.wikipedia.org/wiki/List_of_UTC_time_offsets

- 1	UTC−12:00, 	
- 2	UTC−11:00, 	
- 3	UTC−10:00, 	
- 4	UTC−09:30, 
- 5	UTC−09:00, 	
- 6	UTC−08:00, 
- 7	UTC−07:00, 
- 8	UTC−06:00, 
- 9	UTC−05:00, 
- 10	UTC−04:00, 
- 11	UTC−03:30, 
- 12	UTC−03:00, 
- 13	UTC−02:00, 
- 14	UTC−01:00, 
- 15	UTC±00:00, 
- 16	UTC+01:00, 
- 17	UTC+02:00, 
- 18	UTC+03:00, 
- 19	UTC+03:30, 
- 20	UTC+04:00, 
- 21	UTC+04:30, 
- 22	UTC+05:00, 
- 23	UTC+05:30, 
- 24	UTC+05:45, 
- 25	UTC+06:00, 
- 26	UTC+06:30, 
- 27	UTC+07:00, 
- 28	UTC+08:00, 
- 29	UTC+08:45, 
- 30	UTC+09:00, 
- 31	UTC+09:30, 
- 32	UTC+10:00, 	
- 33	UTC+10:30, 	
- 34	UTC+11:00, 	
- 35	UTC+12:00, 
- 36	UTC+12:45, 	
- 37	UTC+13:00, 	
- 38	UTC+14:00,    

 - Given the complexity of how timeworks, there are likely errors and inconsistencies with most/all of these and none of this data should be considered “official”.
