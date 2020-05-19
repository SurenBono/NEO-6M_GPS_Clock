# NEO-6M-GPS-TIME
NEO-6M-GPS-CLOCK-DATE-LAT-LONG-SPEED-ALTITUDE-SAT USED

- Tested on chinese SMD ATMEL UNO
- Hard soldered 3 wire underside of UNO for Neo-6M with MCUFRIEND TFT display
- 3v3 to Neo VCC
- GND Neo to any common UNO GND
- NEO can only receive data ,NEO TX to UNO RX
- Unplug tx wire while upload to avoid upload error
- Download include drivers to library
- Feel free to try on other display..Led Matrix..etc
- Automatically adjust according to device located timezone via neofix drivers calculation 
- GMT/UTP input can be ignored

TIMEZONE LIST
-----------------
https://en.wikipedia.org/wiki/List_of_UTC_time_offsets

1	UTC−12:00, 	
2	UTC−11:00, 	
3	UTC−10:00, 	
4	UTC−09:30, 
5	UTC−09:00, 	
6	UTC−08:00, 
7	UTC−07:00, 
8	UTC−06:00, 
9	UTC−05:00, 
10	UTC−04:00, 
11	UTC−03:30, 
12	UTC−03:00, 
13	UTC−02:00, 
14	UTC−01:00, 
15	UTC±00:00, 
16	UTC+01:00, 
17	UTC+02:00, 
18	UTC+03:00, 
19	UTC+03:30, 
20	UTC+04:00, 
21	UTC+04:30, 
22	UTC+05:00, 
23	UTC+05:30, 
24	UTC+05:45, 
25	UTC+06:00, 
26	UTC+06:30, 
27	UTC+07:00, 
28	UTC+08:00, 
29	UTC+08:45, 
30	UTC+09:00, 
31	UTC+09:30, 
32	UTC+10:00, 	
33	UTC+10:30, 	
34	UTC+11:00, 	
35	UTC+12:00, 
36	UTC+12:45, 	
37	UTC+13:00, 	
38	UTC+14:00,    
