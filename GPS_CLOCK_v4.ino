
/*
   FX GPS Clock + Weekday decoder via valid GPS date (localised calculation) 

- A rare sketch Since Weekday are not included in NMEAGPS . Originally written by Eric Sitler 30August2016
- Mod to your timezone line 59
- Add prefered FX 

*/
 
#include <SPI.h>
#include <math.h>
#include <NMEAGPS.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <NeoSWSerial.h>

//#include "Parola_Fonts_data.h"
//#include "Parola_Sat_Font.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN 11	    //13 if esp8266
#define CS_PIN   10	    //15
#define CLK_PIN  13	    //14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
textEffect_t FX[] ={PA_SCROLL_LEFT,PA_SCROLL_RIGHT,PA_OPENING_CURSOR};

NMEAGPS gps;  
gps_fix fix;  
uint8_t fixCount;
NeoSWSerial serialgps(A5,0);

short h;  	          //  Global localised variable declaration for 12-hour AM/PM format
short m;		      // Minute variable declaration
short s;		      // Second variable declaration
short d;		      // date
short mth;  	      // month   
short yy;             // Last 2 digits of the year (ie 2016 would be 16)
short yyyy;           // Year Entry
short c;              // Century (ie 2016 would be 20)
short mTable;         // Month value based on calculation table
short SummedDate;     // Add values combined in prep for Mod7 calc
short DoW;            // Day of the week value (0-6)
short leap;           // Leap Year or not
short cTable;         // Century value based on calculation table
short i=0;            // Array shifter count
short counter=0;	  // Animation control count
String Mdn;     	  // Variable AM/PM
String DAYX;          // Stringyfied strftime later charred for display justification Parola rules.
String TIMEX;
String DATEX;
char dayxx[15];
char timexx[15];
char datexx[15];

const char *GPS[3]={dayxx,timexx,datexx};    //Arrays sequences in Pointed Array style
const uint16_t ONE_SEC = 1000;               // Parola did not respond to delay(1000)
const int32_t zone_hours= +8L; 	// Covert Timezone into seconds ,60 sec * 60 min * Timezone rule

char Time [] = ":00:00";        // Minute & Second array , no leading zero Hour rule

void setup(void)
{
  Serial.begin(9600);			        // experimented 1st on serial monitor before migrating to FC-16 & TFT
  serialgps.begin(9600); 		        // Neo SwSerial init
  P.begin();				            // Parola lib init
//P.setFont(ExtASCII);                  // for Custom fonts in "Parola_Sat_Font.h"...etc
  P.setIntensity(0);			        // Lowest LED brightness
  P.setTextAlignment(PA_CENTER);	    // Centered display
  P.print("GPS CLOCK");
  Serial.println("GPS CLOCK");
  delay(ONE_SEC);						// Parola did not respond to delay(1000)
  P.displayClear(); 
}

void adjustTime( NeoGPS::time_t & dt )
{
  NeoGPS::clock_t seconds = dt;     // convert NMEA date/time structure to seconds
  const NeoGPS::clock_t  zone_offset = zone_hours * NeoGPS::SECONDS_PER_HOUR;   // -/+ timezone processing
  seconds += zone_offset;			// DST ,recommends if statement addition if hour shifts during valid month
  dt = seconds;                     // convert seconds structure back to date/time
} 

void loop(){
  
if (gps.available( serialgps ))
  { 
    fix = gps.read();fixCount++;
    if (fixCount >= 1)  // Simple quick clock calibration , Coordinate decoding needs >=5 fixCount to process(uno)
    { 
      fixCount = 0; 
      if (fix.valid.time)adjustTime(fix.dateTime);printGPSdata();
    }
  }while (!P.displayAnimate());		// Parola FX command
}


void printGPSdata()
{ 
 
  if (fix.valid.time&&fix.dateTime.hours>11){h=fix.dateTime.hours-12;Mdn="PM ";}
  else{h=fix.dateTime.hours;Mdn="AM ";}
  if (fix.valid.time&&fix.dateTime.hours==0||h==0) {h=12;}
  m=fix.dateTime.minutes;
  s=fix.dateTime.seconds;
  if (fix.valid.date) {d= fix.dateTime.date ;mth=fix.dateTime.month ;yyyy=fix.dateTime.year;}
 
	Time[1]  = m  / 10 +'0';
	Time[2]  = m  % 10 +'0';	
    Time[4]  = s  / 10 +'0';
    Time[5]  = s  % 10 +'0';
    
	String H=String(h); TIMEX=Mdn+H+Time;   //Align Clock and combine int&char into a string
	
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

    if(DoW == 0) { DAYX="Saturday"  ; }				// Results of Weekday calculation
    if(DoW == 1) { DAYX="Sunday"    ; }
    if(DoW == 2) { DAYX="Monday"    ; }
    if(DoW == 3) { DAYX="Tuesday"   ; }
    if(DoW == 4) { DAYX="Wednesday" ; }
    if(DoW == 5) { DAYX="Thursday"  ; }
    if(DoW == 6) { DAYX="Friday"    ; }
	
	DATEX=String(d); 			//Arrange & stringfy local date decoded by valid date & +/- Timezone fix
	DATEX+=".";
	DATEX+=String(mth);
	DATEX+=".";
    DATEX+=String(yyyy);
 
 Serial.print(DAYX);Serial.print(" ,");Serial.print(TIMEX);Serial.print(" ,");Serial.println(DATEX);
 DAYX.toCharArray(dayxx, 15);TIMEX.toCharArray(timexx, 15);DATEX.toCharArray(datexx, 15);//Coverts string toCharArray
 // P.print(GPS[i]);         // initial Simple no fx test
 if(i>2){ i=0;counter=0;}    // Array sequencing 
 P.displayText(GPS[i],PA_CENTER , 10, 2000, FX[i], FX[i]);//*Array,TextAlign,AnimSpeed,PauseTime,inFx,outFx
 i++; 

}
// end of code. Developed by Sroto&Gargees 2020
