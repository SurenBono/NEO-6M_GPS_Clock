// ESP-NOW ESP8266 RECEIVER

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
textEffect_t FX[] ={PA_SCROLL_LEFT,PA_PRINT,PA_SCROLL_RIGHT};

String mr,hr,mn,sec,sat=String(char(127)),t,t1,Dayname,SaT=String(char(129)),utc="UTC+8 "+SaT;
char T[15],dd[30];
const char *GPS[2]={T,dd};
int i;

char Time []   = ":00:00"; 
char Time_b [] = " 00 00";  

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
  P.print("GPS Rx");
  Serial.println("GPS Rx");
   delay(1000); 
  
  if (esp_now_init() != 0) {Serial.println("Reboot GPS Rx "); return;}
  esp_now_register_recv_cb(onDataReceiver);
  }

void onDataReceiver(uint8_t * mac, uint8_t *incomngData, uint8_t len){
   
memcpy(&myMessage, incomngData, sizeof(myMessage));
  
  if (myMessage.W==1){Dayname="Sunday";}
  if (myMessage.W==2){Dayname="Monday";}
  if (myMessage.W==3){Dayname="Tuesday";}
  if (myMessage.W==4){Dayname="Wednesday";}
  if (myMessage.W==5){Dayname="Thursday";}
  if (myMessage.W==6){Dayname="Friday";}
  if (myMessage.W==7){Dayname="Saturday";}
  
      Time[1]    = myMessage.m / 10 +'0';
      Time[2]    = myMessage.m  % 10 +'0';  
      Time[4]    = myMessage.s / 10 +'0';
      Time[5]    = myMessage.s % 10 +'0';
    
      Time_b[1]  = myMessage.m  / 10 +'0';
      Time_b[2]  = myMessage.m  % 10 +'0';  
      Time_b[4]  = myMessage.s / 10 +'0';
      Time_b[5]  = myMessage.s  % 10 +'0';
	  
  
  Serial.print(Dayname);
  Serial.print("  ");

  if (myMessage.M==0){Serial.print("AM ");mr="AM ";}else{Serial.print("PM ");mr="PM ";}
  Serial.print(myMessage.h); Serial.print(":");hr=myMessage.h;
  
  t =sat+" "+mr+hr+Time;
  t1=sat+" "+mr+hr+Time_b;;
  
  if(myMessage.m<10){mn='0';mn += String (myMessage.m);}else{mn=myMessage.m;}Serial.print(mn);Serial.print(":");
  if(myMessage.s<10){sec='0';sec += String (myMessage.s);}else{sec=myMessage.s;}Serial.print(sec); 
  
  Serial.print("  ");
  Serial.print(myMessage.d);Serial.print(".");
  Serial.print(myMessage.mt);Serial.print(".");Serial.println(myMessage.y);
  
  Dayname +="   " + String(myMessage.d) + "." + String(myMessage.mt) + "." +String(myMessage.y)+"   "+utc;
  Dayname.toCharArray(dd,30);t.toCharArray(T,15);

}

void sequences(){
P.setTextAlignment(PA_LEFT);
switch (i) 
  {
     case 1 :P.print(t);delay(500);P.print(t1);delay(500);break;  case 2 :P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 3 :P.print(t);delay(500);P.print(t1);delay(500);break;  case 4 :P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 5 :P.print(t);delay(500);P.print(t1);delay(500);break;  case 6 :P.print(t);delay(500);P.print(t1);delay(500);break;
     case 7 :P.print(t);delay(500);P.print(t1);delay(500);break;  case 8 :P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 9 :P.print(t);delay(500);P.print(t1);delay(500);break;  case 10:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 11:P.print(t);delay(500);P.print(t1);delay(500);break;  case 12:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 13:P.print(t);delay(500);P.print(t1);delay(500);break;  case 14:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 15:P.print(t);delay(500);P.print(t1);delay(500);break;  case 16:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 17:P.print(t);delay(500);P.print(t1);delay(500);break;  case 18:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 19:P.print(t);delay(500);P.print(t1);delay(500);break;  case 20:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 21:P.print(t);delay(500);P.print(t1);delay(500);break;  case 22:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 23:P.print(t);delay(500);P.print(t1);delay(500);break;  case 24:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 25:P.print(t);delay(500);P.print(t1);delay(500);break;  case 26:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 27:P.print(t);delay(500);P.print(t1);delay(500);break;  case 28:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 29:P.print(t);delay(500);P.print(t1);delay(500);break;  case 30:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 31:P.print(t);delay(500);P.print(t1);delay(500);break;  case 32:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 33:P.print(t);delay(500);P.print(t1);delay(500);break;  case 34:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 35:P.print(t);delay(500);P.print(t1);delay(500);break;  case 36:P.print(t);delay(500);P.print(t1);delay(500);break;  
     case 37:P.print(t);delay(500);P.print(t1);delay(500);break;  case 38:P.print(t);delay(500);P.print(t1);delay(500);break;
     case 39:P.print(t);delay(500);P.print(t1);delay(500);break;  case 40:P.print(t);delay(500);P.print(t1+"    ");break;
   
     case 41:P.displayText(dd,PA_CENTER ,17,0, FX[0], FX[0]);break;
     case 42:P.displayText(T,PA_LEFT  ,10,25, FX[2], FX[1]);break;
     
     default:P.print(t);
    }
   i++;if(i>42){i=1;}
}  
	  
void loop() {
 while(P.displayAnimate())sequences();
}
// end of code
