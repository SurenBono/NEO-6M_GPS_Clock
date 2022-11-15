// ESP-NOW ESP8266 GPS TRANSMITTER
// Neo-6M Tx --> esp Rx ( Fast HW serial)
// Connect Tx --> Rx after uploading + reset
// Minimalist Serial monitor activity, reports 1 if no device detected yet
// It takes hours to get a fix sometimes but once locked it flows..
// The objective of this project is to sync all Clock on the receiver side.

#include <Arduino.h>          // the libs are arranged in this sequence or it errors
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "TimeLib.h"          // add these lib into project folder if possible 
#include "TinyGPSPlus.h"      

#define time_offset   8

uint8_t peer1[] = {0x80, 0xXX, 0x3A, 0xXX, 0xF7, 0xX6};  // Find & add your device MAC
uint8_t peer2[] = {0xXC, 0xF4, 0x32, 0xXX, 0xX5, 0xC6};  // 

byte last_second, Second, Minute, Hour, Day, Month;
int Year;

typedef struct message {
  int W, h, m, s, d, mt, y, M ;
};

TinyGPSPlus gps;
struct message myMessage;

void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus);
}

void setup() {

  Serial.begin(9600);
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
  while (Serial.available() > 0)
  {
    if (gps.encode(Serial.read()))
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

        if (weekday() == 1) {
Serial.print("Sunday");
    }
        if (weekday() == 2) {
Serial.print("Monday");
    }
        if (weekday() == 3) {
Serial.print("Tuesday");
   }
        if (weekday() == 4) {
Serial.print("Wednesday");
 }
        if (weekday() == 5) {
Serial.print("Thursday");
  }
        if (weekday() == 6) {
Serial.print("Friday");
    }
        if (weekday() == 7) {
Serial.print("Saturday");
  }

        Serial.print("  ");
        if (isAM()) {
          myMessage.M = 0;
          Serial.print("AM");
        } else {
          myMessage.M = 1;
          Serial.print("PM");
        }
        Serial.print(hourFormat12());
        if (minute() < 10)Serial.print('0'); Serial.print(minute());
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
  delay(1000);
}
// end of code
