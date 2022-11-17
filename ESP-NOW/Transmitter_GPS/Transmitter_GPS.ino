// ESP-NOW ESP8266 GPS TRANSMITTER
// Minimalist Serial monitor activity, reports 1 if no device detected yet
// Once fix/locked it flows..place near windows for faster transmitter clock fix/auto adjustments 
// The objective of this project is to sync all Clock on the receiver side.
// Preview https://youtu.be/Evq018rAHy0
// https://youtu.be/HKpepkdoFHM

#include <Arduino.h>          // the libs are arranged in this sequence to avoid errors
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h> 
#include "TimeLib.h"          // add these lib into project folder if possible 
#include "TinyGPSPlus.h"      

#define time_offset   8       // Adjust to your timezone

uint8_t peer1[] = {0x80, 0xXX, 0x3A, 0xXX, 0xF7, 0xX6};  // Find & add your device MAC
uint8_t peer2[] = {0xXC, 0xF4, 0x32, 0xXX, 0xX5, 0xC6};  // 

byte last_second, Second, Minute, Hour, Day, Month;
int Year;

typedef struct message {
  int W, h, m, s, d, mt, y, M ;
};

TinyGPSPlus gps;
struct message myMessage;

SoftwareSerial NEO(13,15);  //Esp (RX,TX)--> NEO TX(EspRX2,13/D7),NEO RX(EspTX2,15/D8)

void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus);
}

void setup() {

  Serial.begin(9600);
  NEO.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println("Transmitter");

  if (esp_now_init() != 0) {
    Serial.println("Reboot Transmitter ");
    return;

  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_add_peer(peer1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(peer2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_register_send_cb(onSent);
}

void prn_GPS_Local()
{
  while (NEO.available() > 0)
  {
    if (gps.encode(NEO.read()))
    {
      if (gps.time.isValid())
      {
        Hour   = gps.time.hour();
        Minute = gps.time.minute();
        Second = gps.time.second();
      }

      if (gps.date.isValid())
      {
        Day   = gps.date.day();
        Month = gps.date.month();
        Year  = gps.date.year();
      }

      if (last_second != gps.time.second())
      {
        last_second = gps.time.second();
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(time_offset * SECS_PER_HOUR);

        if (weekday() == 1) {Serial.print("Sunday");}
        if (weekday() == 2) {Serial.print("Monday"); }
        if (weekday() == 3) {Serial.print("Tuesday"); }
        if (weekday() == 4) {Serial.print("Wednesday");}
        if (weekday() == 5) {Serial.print("Thursday"); }
        if (weekday() == 6) {Serial.print("Friday"); }
        if (weekday() == 7) {Serial.print("Saturday"); }

        Serial.print("  ");
        if (isAM()) {myMessage.M = 0;Serial.print("AM"); } else { myMessage.M = 1;Serial.print("PM");  }

        Serial.print(" ");
        Serial.print(hourFormat12());Serial.print(":");
        if (minute() < 10)Serial.print('0'); Serial.print(minute());Serial.print(":");
        if (second() < 10)Serial.print('0'); Serial.print(second());
        Serial.print("  ");
        Serial.print(day());
        Serial.print("/");
        Serial.print(month());
        Serial.print("/");
        Serial.print(year());
        Serial.println();

      }
    }
  }
}

void transmit() {

  myMessage.W = weekday();
  myMessage.M ;
  myMessage.h  = hourFormat12();
  myMessage.m  = minute();
  myMessage.s  = second();
  myMessage.d  = day();
  myMessage.mt = month();
  myMessage.y  = year();

  esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
}

void loop() {

  prn_GPS_Local();
  transmit() ;
  delay(100);   // 100 millis apart from Stratum 1 , unstable broadcast beyond this , supports <10 device reception 
}
// end of code
