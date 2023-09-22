

#include <SPI.h>            
#include <MD_Parola.h>            
#include <MD_MAX72xx.h>  */
#include <SoftwareSerial.h>

#include "Sat_Fontx.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 7      // 2pcs * 4 Led Matrix , Vcc= 5V + GND (5wire)
#define DATA_PIN    11     // UNO SPI 11 MOSI ,MEGA  51
#define CS_PIN      10     // UNO SPI 10 SS   ,MEGA  53
#define CLK_PIN     13     // UNO SPI 13 SCK  ,MEGA  52*/
#define Pz          7     // Piezo VCC + GND     (2pin ,stick direct to Mega)
#define f           4096   // Piezo Tone ic C note  
#define GND         4

 char rawData[64]="";
 char *token;
 char delimiter[] = ",";
 char tokenStorage[5][20]; 
 char time_b[15],date[10],wday[10];
 int i;
 String beep,sub;
 bool flag;

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
textEffect_t FX[] ={PA_SCROLL_LEFT,PA_PRINT,PA_SCROLL_RIGHT};

const uint16_t ONE_SEC = 1000; 
const uint16_t HALF_SEC = 500; 

SoftwareSerial rx(2,3);     //NEO TX == >UNO RX

void setup(void)
{
  Serial.begin(9600);            
  rx.begin(9600); 
  P.begin();                   
  P.setFont(ExtASCII);   
  P.setIntensity(0);              
  P.setTextAlignment(PA_CENTER);      
  P.print("S&G GPS v10"); 
    
 pinMode(Pz, OUTPUT);
 pinMode(GND, OUTPUT);
 digitalWrite(Pz,LOW);
 digitalWrite(GND,LOW);
    
    Serial.println("S&G GPS Clock V10");
    tone(Pz,f);delay(30);noTone(Pz);delay(30);
    tone(Pz,f);delay(30);noTone(Pz);delay(30);
    tone(Pz,f);delay(30);noTone(Pz);
    
  delay(ONE_SEC);           
  P.displayClear();

}

void stow(){
  
size_t byteCount = rx.readBytesUntil('\n', rawData, sizeof(rawData) - 1); 
rawData[byteCount] = NULL;

token = strtok(rawData, delimiter);
int tokenIndex = 0;
while (token != NULL && tokenIndex <= 5) {
strcpy(tokenStorage[tokenIndex], token);
tokenIndex++;
token = strtok(NULL, delimiter);
 }
  strcpy(date,tokenStorage[0]);
  strcpy(wday,tokenStorage[1]);
  strcpy(time_b,tokenStorage[2]);

  if (time_b[4]==':'){time_b[4]=' ';time_b[7]=' ';}
  else{time_b[5]=' ';time_b[8]=' ';}

  beep=String (tokenStorage[2]);
  sub=beep.substring(5,11);
  
    if(sub==":00:00" && flag==0 || sub=="00:00" && flag==0){ 
    tone(Pz,f);delay(30);noTone(Pz);delay(30);
    tone(Pz,f);delay(30);noTone(Pz);delay(30);flag=1;
    }else{flag=0;}

    if(sub==":30:00" && flag==0 || sub=="30:00" && flag==0){ 
    tone(Pz,f);delay(30);noTone(Pz);delay(30);flag=1;
    }else{flag=0;}

  Serial.print(date);Serial.print(",");Serial.print(wday);Serial.print(",");
  Serial.println(tokenStorage[2]);
  
  seq();
}
  void seq(){
  P.setTextAlignment(PA_LEFT);
  switch (i) 
  {
     case 1 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 2 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 3 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 4 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 5 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 6 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 7 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 8 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 9 :P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 10:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 11:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 12:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 13:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 14:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 15:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 16:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 17:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 18:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 19:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 20:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 21:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 22:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 23:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 24:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 25:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 26:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 27:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 28:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 29:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 30:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 31:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 32:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 33:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 34:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 35:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 36:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  
     case 37:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 38:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
     case 39:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;  case 40:P.print(tokenStorage[2]);delay(HALF_SEC);P.print(time_b);break;
   
     case 41:P.print(wday);delay(ONE_SEC);break;
     case 42:P.print(date);delay(ONE_SEC);break;
     
     default:P.print(tokenStorage[2]);
    }
   i++;if(i>42){i=1;}
}
  
  
  void loop(){ stow(); }
  
