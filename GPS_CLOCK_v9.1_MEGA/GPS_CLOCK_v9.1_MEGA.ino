
/*
- FX GPS Clock + Weekday decoder + Piezo + dht11 via valid GPS date (localised calculation) 9 wire.
- The sketch was to big to fit uno for piezo & dht11 so tried Mega with its extra hardware Serial ports.
- New Hourly Piezo Chiming GPS Clock  on Mega hardware Serial1.
- A rare sketch Since Weekday are not included in NMEAGPS . Originally written by Eric Sitler 30August2016
- Mod to your timezone line 43
- Add prefered FX for led display.
- https://pjrp.github.io/MDParolaFontEditor
*/
 
#include <SPI.h>
#include <math.h>             
#include <NMEAGPS.h>              // NeoGPS-4.2.9     https://www.arduino.cc/reference/en/libraries/neogps/
#include <MD_Parola.h>            // MD_Parola-3.6.1  https://www.arduino.cc/reference/en/libraries/md_parola/
#include <MD_MAX72xx.h>           // https://www.arduino.cc/reference/en/libraries/md_max72xx/

#include "Sat_Fontx.h"            // Mod your font https://pjrp.github.io/MDParolaFontEditor

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW    //adjust to your Led Matrix ,4 types listed

#define MAX_DEVICES 8      // 2pcs * 4 Led Matrix , Vcc= 5V + GND (5wire)
#define DATA_PIN    51     // UNO SPI 11 MOSI
#define CS_PIN      53     // UNO SPI 10 SS
#define CLK_PIN     52     // UNO SPI 13 SCK 
#define Pz          11     // Piezo VCC + GND     (2pin ,stick direct to Mega)
#define f           4096   // Piezo Tone ic C note  

//NEO TX == >MEGA RX (pin19) + Vcc 3V3 + GND on MEGA Serial2 (3wire)

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
textEffect_t FX[] ={PA_SCROLL_LEFT,PA_PRINT,PA_SCROLL_RIGHT};

NMEAGPS gps;  
gps_fix fix; 
uint8_t fixCount;
 
const uint16_t ONE_SEC = 1000; 
const uint16_t HALF_SEC = 500;  
const int32_t zone_hours= +8;    
            
int h,m,s,d,mth,yy,yyyy,c,mTable,SummedDate,DoW,leap,cTable,i=1,j,bip=0;             
String Mdn,DAYX,TIMEX,TIMEY,DATEX,UTC=" U+",LOGO = (char(127))+UTC+zone_hours+" "+(char(129)),Sat=String(char(127)),SAT=String(char(129));       
char dayxx[55],timexx[15],datexx[25],logo[25];
const char *GPS[5]={dayxx,timexx,datexx,logo};    

char Time []   = ":00:00"; 
char Time_b [] = " 00 00";             

void setup(void)
{
  Serial.begin(9600);            
  Serial1.begin(9600);           
  P.begin();                   
  P.setFont(ExtASCII);   
  P.setIntensity(0);              
  P.setTextAlignment(PA_CENTER);      
  P.print("S&G GPS v9");                     
   
 pinMode(Pz, OUTPUT);
 digitalWrite(Pz,LOW);
    
    Serial.println("S&G GPS Clock V9");
    tone(Pz,f);delay(30);noTone(Pz);delay(30);
    tone(Pz,f);delay(30);noTone(Pz);delay(30);
    tone(Pz,f);delay(30);noTone(Pz);
    
  delay(ONE_SEC);           
  P.displayClear();
}

void adjustTime( NeoGPS::time_t & dt )
{
  NeoGPS::clock_t seconds = dt;     
  const NeoGPS::clock_t  zone_offset = zone_hours * NeoGPS::SECONDS_PER_HOUR;   
  seconds += zone_offset;     
  dt = seconds;                     
} 

void loop(){
  
if (gps.available( Serial1 ))
   { 
     fix = gps.read();fixCount++;
      if (fixCount >= 1)  
       { 
         fixCount = 0; 
         if (fix.valid.time)adjustTime(fix.dateTime);printGPSdata();
       }
   } while (!P.displayAnimate());   
}

void printGPSdata()
{ 
 
  if (fix.valid.time&&fix.dateTime.hours>11){h=fix.dateTime.hours-12;Mdn="PM ";}
  else{h=fix.dateTime.hours;Mdn="AM ";}
  if (fix.valid.time&&fix.dateTime.hours==0||h==0) {h=12;}
  m=fix.dateTime.minutes;
  s=fix.dateTime.seconds;

  if ( m==0 && s==0 && bip==0){ 
    tone(Pz,f);delay(50);noTone(Pz);delay(50);tone(Pz,f);delay(50);noTone(Pz);bip=1;}  //Beep twice hourly

 if  ( m==30 && s==0 && bip==0){ 
     tone(Pz,f);delay(50);noTone(Pz);bip=1;}   //Beep once every 30min  
	 
 if ( s==1 && bip==1){ bip=0;} // chime control on 1/2 sec update
  
  
 if (fix.valid.date) {d= fix.dateTime.date ;mth=fix.dateTime.month ;yyyy=fix.dateTime.year;}
 
      Time[1]    = m  / 10 +'0';
      Time[2]    = m  % 10 +'0';  
      Time[4]    = s  / 10 +'0';
      Time[5]    = s  % 10 +'0';
    
      Time_b[1]  = m  / 10 +'0';
      Time_b[2]  = m  % 10 +'0';  
      Time_b[4]  = s  / 10 +'0';
      Time_b[5]  = s  % 10 +'0';
    
  String H = String(h); 
  String TIMEZ=Mdn+H+Time;
  TIMEX=SAT +" "+Mdn+H+Time;   //Align Clock and combine int&char into a string
  TIMEY=SAT +" "+Mdn+H+Time_b;
  
  if((fmod(yyyy,4) == 0 && fmod(yyyy,100) != 0) || (fmod(yyyy,400) == 0)) { leap = 1; } // Weekday calculation starts
  else { leap = 0; }

    while(yyyy > 2299){ yyyy = yyyy - 400; }
    while(yyyy < 1900){ yyyy = yyyy + 400; }

    c = yyyy/100;

    yy = fmod(yyyy, 100);

    if(c == 19){ cTable = 1; }
    if(c == 20){ cTable = 0; }
    if(c == 21){ cTable = 5; }
    if(c == 22){ cTable = 3; }

    if(mth == 1){ if(leap == 1) { mTable = 6; }else{ mTable = 0; }}
    if(mth == 2){ if(leap == 1) { mTable = 2; }else{ mTable = 3; }}
 
    if(mth == 10){ mTable = 0; }
    if(mth == 8 ){ mTable = 2; }
    if(mth == 3 || mth == 11   ) { mTable = 3; }
    if(mth == 4 || mth == 7    ) { mTable = 6; }
    if(mth == 5 ){ mTable = 1; }
    if(mth == 6 ){ mTable = 4; }
    if(mth == 9 || mth == 12   ) { mTable = 5; }

    SummedDate = d + mTable + yy + (yy/4) + cTable;
    
    DoW = fmod(SummedDate,7);  

    if(DoW == 0) { DAYX="Saturday"  ; }       // Results of Weekday calculation
    if(DoW == 1) { DAYX="Sunday"    ; }
    if(DoW == 2) { DAYX="Monday"    ; }
    if(DoW == 3) { DAYX="Tuesday"   ; }
    if(DoW == 4) { DAYX="Wednesday" ; }
    if(DoW == 5) { DAYX="Thursday"  ; }
    if(DoW == 6) { DAYX="Friday"    ; }
  
  DATEX=String(d);      
  DATEX+=".";
  DATEX+=String(mth);
  DATEX+=".";
  DATEX+=String(yyyy);
   
 LOGO.toCharArray(logo, 15);
 
 Serial.print(TIMEZ);Serial.print(" ,");Serial.print(DAYX);Serial.print(" ,");Serial.println(DATEX);
 DAYX+="    "+ DATEX +"  "+LOGO; DAYX.toCharArray(dayxx,55);
 TIMEX.toCharArray(timexx, 15);
 sequences();
}

void sequences(){
P.setTextAlignment(PA_LEFT);
switch (i) 
  {
     case 1 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 2 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 3 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 4 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 5 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 6 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 7 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 8 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 9 :P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 10:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 11:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 12:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 13:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 14:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 15:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 16:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 17:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 18:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 19:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 20:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 21:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 22:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 23:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 24:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 25:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 26:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 27:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 28:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 29:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 30:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 31:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 32:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 33:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 34:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 35:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 36:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  
     case 37:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 38:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;
     case 39:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY);break;  case 40:P.print(TIMEX);delay(HALF_SEC);P.print(TIMEY+"    ");break;
   
     case 41:P.displayText(dayxx,PA_CENTER ,20,0, FX[0], FX[0]);break;
     case 42:P.displayText(timexx,PA_LEFT  ,15,25, FX[2], FX[1]);break;
     
     default:P.print(TIMEX);
    }
   i++;if(i>42){i=1;}
}
// end of code. Developed by Sroto&Gargees 2022
