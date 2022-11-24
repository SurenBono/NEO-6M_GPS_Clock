
/* STM32 Bluepill Not Recommended,the module only worked once and a piece of trash by now..
   
FX GPS Clock + Weekday decoder via valid GPS date (localised calculation) on stm32f103c8t6 aka "BLUEPILL"

- A rare sketch Since Weekday are not included in NMEAGPS . Originally written by Eric Sitler 30August2016
- Mod to your timezone line 36
- Add prefered FX 
- https://pjrp.github.io/MDParolaFontEditor
*/
 
#include <SPI.h>
#include <math.h>
#include <NMEAGPS.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <HardwareSerial.h>
#include "Sat_Fontx.h"


#define gpsPort Serial3
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN    PA7      //PB5 ,PA7   STM32 MOSI   /13 if esp8266 / uno 11
#define CS_PIN      PA4      //PA15,PA4   STM32 SS     /15 if esp8266 / uno 10
#define CLK_PIN     PA5      //PB3 ,PA5   STM32 SCK    /14 if esp8266 / uno 13

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
textEffect_t FX[] ={PA_SCROLL_LEFT, PA_PRINT,PA_SCROLL_RIGHT};

NMEAGPS gps;  
gps_fix fix;  
uint8_t fixCount;
HardwareSerial Serial3 (PB11, PB10);       

const uint16_t ONE_SEC = 1000; 
const uint16_t HALF_SEC = 500;  
const int32_t zone_hours= +8L;    
            
int h,m,s,d,mth,yy,yyyy,c,mTable,SummedDate,DoW,leap,cTable,i=1,j ; 	          
String Mdn,DAYX,TIMEX,TIMEY,DATEX, UTC=" UTC+",LOGO = (char(127))+UTC+zone_hours+" "+(char(129));    	  
char dayxx[45],timexx[15],datexx[25],logo[25];
const char *GPS[4]={dayxx,timexx,datexx,logo};    

char Time []   = ":00:00"; 
char Time_b [] = " 00 00";             

void setup(void)
{
  Serial.begin(9600);	
  Serial3.begin(9600);	       
  P.begin();				           
  P.setFont(ExtASCII);   
  P.setIntensity(0);			        
  P.setTextAlignment(PA_CENTER);	    
  P.print("S&G GPS");                     
  Serial.println("S&G GPS Clock v8.2");
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

void printGPSdata()
{ 
 
  if (fix.valid.time&&fix.dateTime.hours>11){h=fix.dateTime.hours-12;Mdn="PM ";}
  else{h=fix.dateTime.hours;Mdn="AM ";}
  if (fix.valid.time&&fix.dateTime.hours==0||h==0) {h=12;}
  m=fix.dateTime.minutes;
  s=fix.dateTime.seconds;
  if (fix.valid.date) {d= fix.dateTime.date ;mth=fix.dateTime.month ;yyyy=fix.dateTime.year;}
 
      Time[1]    = m  / 10 +'0';
      Time[2]    = m  % 10 +'0';  
      Time[4]    = s  / 10 +'0';
      Time[5]    = s  % 10 +'0';
    
      Time_b[1]  = m  / 10 +'0';
      Time_b[2]  = m  % 10 +'0';  
      Time_b[4]  = s  / 10 +'0';
      Time_b[5]  = s  % 10 +'0';
    
  String H = String(h);    //Align Clock and combine int&char into a string
  
  
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

 //LOGO = (char(127))+UTC + zone_hours+" "+(char(129)); 
 LOGO.toCharArray(logo, 15);
 
 Serial.print(TIMEX);Serial.print(" ,");Serial.print(DAYX);Serial.print(" ,");Serial.println(DATEX);
 DAYX+="    "+ DATEX + "    " + LOGO; DAYX.toCharArray(dayxx, 45);TIMEX=Mdn+H+Time;TIMEY=Mdn+H+Time_b;
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
   
     case 41:P.displayText(dayxx,PA_CENTER ,30,0, FX[0], FX[0]);break;
     case 42:P.displayText(timexx,PA_LEFT  ,15,25, FX[2], FX[1]);break;
     
     default:P.print(TIMEX);
    }
   i++;if(i>42){i=1;}
}
void loop(){
  
if  (gps.available( Serial3 )) 
   { 
     fix = gps.read();fixCount++;
      if (fixCount >= 1)  
       { 
         fixCount = 0; 
         if (fix.valid.time)adjustTime(fix.dateTime);printGPSdata();
       }
   } while (!P.displayAnimate());		
}

// end of code. Developed by Sroto&Gargees 2020
