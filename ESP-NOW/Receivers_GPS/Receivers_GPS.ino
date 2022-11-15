// Still working on it...
// Soon it will have scrolling display
// ESP-NOW ESP8266 RECEIVERs Side


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "Sat_Fontx.h" 

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

#define DATA_PIN   13  //MOSI D11/D7(D1R1)
#define CS_PIN     15  //SS   D10   (D1R1)
#define CLK_PIN    14  //SCK  D13/D5(D1R1)

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

String mn,sec,t=String(char(127)),Dayname;

typedef struct message {
   int W,h,m,s,d,mt,y,M ;
} message;

message myMessage;

void setup() {

  Serial.begin(9600);
  WiFi.disconnect();
  ESP.eraseConfig();
  WiFi.mode(WIFI_STA);
  P.begin();
  P.setFont(ExtASCII);   
  P.setIntensity(0);              
  P.setTextAlignment(PA_CENTER);  
  P.print("GPS R1");
  Serial.println("GPS R1");
  
  if (esp_now_init() != 0) {Serial.println("Reboot GPS R1 "); return;}
  esp_now_register_recv_cb(onDataReceiver);
  }

void onDataReceiver(uint8_t * mac, uint8_t *incomngData, uint8_t len){
   
memcpy(&myMessage, incomngData, sizeof(myMessage));
  
  if (myMessage.W==1){Dayname="Sunday   ";}
  if (myMessage.W==2){Dayname="Monday   ";}
  if (myMessage.W==3){Dayname="Tuesday  ";}
  if (myMessage.W==4){Dayname="Wednesday";}
  if (myMessage.W==5){Dayname="Thursday ";}
  if (myMessage.W==6){Dayname="Friday   ";}
  if (myMessage.W==7){Dayname="Saturday ";}

  Serial.print(Dayname);
  Serial.print("  ");

  if (myMessage.M==0){Serial.print("AM ");t="AM ";}else{Serial.print("PM ");t="PM ";}
  Serial.print(myMessage.h); Serial.print(":");t+=String(myMessage.h);t+=(":");
  
  if(myMessage.m<10){mn='0';mn += String (myMessage.m);}else{mn=myMessage.m;}Serial.print(mn); 
  t+=String(mn);
  
  Serial.print(":");t+=(":");
  if(myMessage.s<10){sec='0';sec += String (myMessage.s);}else{sec=myMessage.s;}Serial.print(sec); 
  t+=String(sec);
  
  P.setTextAlignment(PA_LEFT);P.print(t);
  
  //Serial.print(myMessage.s); 
  
  Serial.print("  ");
  Serial.print(myMessage.d);Serial.print(".");
  Serial.print(myMessage.mt);Serial.print(".");Serial.println(myMessage.y);
}

void loop() {delay(1000);}
// end of code
