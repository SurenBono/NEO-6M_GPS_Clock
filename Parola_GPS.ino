/*
Simple +8 UTC GPS Clock
LedMatrix(FC16)+ NEO-6M + SMD UNO + UNO SCREW SHIELD = SOMEWHAT CHEAPEST/SIMPLE STRATUM 1 NON-DST +8 PRECISION GPS CLOCK
Devoloped by http://srotogargees.business.site/

 ** Version 3.1 (+8 UTC GPS Clock) **
 
 - 12-H AM/PM version omitted <10 Hour leading Zero 
 - Note: CLOCK only display demo.
 - PRECISION = +/- 1 Second on lock vs MCU processing speed + display latency.
 - HI-Clock Accuracy compared to NTP >= +/- 500 Millis ~ 1 Second.
 - Allow few minutes,place GPS near windows for fast sat lock. 1 to 5 min for more accurate adjustments (auto)
 - Non-DST +8 GMT/UTC Timezone covers 4 Billion population.
 - Irkutsk,Shanghai,Beijing,Taipei,Hong Kong,Manila,Kuala Lumpur,Singapore,Makassar & Perth.
 - NEO_TX->PIN 10((UNOSS*RX)only rec),NEO_RX->UNOSS*TX can be ignored(not transmitting to sat).
 - or find a software serial pin that will not conflict with display pins.(A5..etc)
 - SS*=Software Serial ,pins that mimics MCU RX&TX to counter upload failure if RX/TX were utilised.
 - Feel free to mod according to your pref(opensourced).STM MCU,OLED,8-Seg,LedMatrix,Serial Monitor...etc
 - Simple/Fast < 150 line Hobbist project.
*/

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <NeoSWSerial.h>
#include <NMEAGPS.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 8
#define DATA_PIN 11	//13 if esp8266
#define CS_PIN   10	//15
#define CLK_PIN  13	//14

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

NMEAGPS gps;  
gps_fix fix;  
uint8_t fixCount;
NeoSWSerial serialgps(A5,0);	//NEOTX->UNOSS*RX,NEORX<-UNOSS*TX

int h;  	//  Global localised variable declaration for 12-hour AM/PM format
int m;
int s;
String Mdn;	//  Variable Meridian AM/PM

const uint16_t ONE_SEC = 1000;
const int32_t zone_hours= +8L; 	// Timezone for non-Daylight Saving zone

char Time [] = "00:00:00";

void setup(void)
{
  //Serial.begin(9600);			// experimented on serial monitor before migrating to FC-16 & TFT
  serialgps.begin(9600); 		// Neo SwSerial init
  P.begin();				// Parola lib init
  P.setIntensity(0);			// Lowest LED brightness
  P.setTextAlignment(PA_CENTER);	// Centered display
  P.print("GPS CLOCK");
   delay(ONE_SEC);
}

void adjustTime( NeoGPS::time_t & dt )
{
  NeoGPS::clock_t seconds = dt;     // convert date/time structure to seconds
  const NeoGPS::clock_t  zone_offset = zone_hours * NeoGPS::SECONDS_PER_HOUR;
  seconds += zone_offset;
  dt = seconds;                     // convert seconds structure back to date/time
} 

void loop()
	
{
	if (gps.available( serialgps ))
	{	
		fix = gps.read();fixCount++;
		if (fixCount >= 1)
		{	
			fixCount = 0;	
			if (fix.valid.time)adjustTime(fix.dateTime);printGPSdata();
		}
	}
}

void printGPSdata()
{ 
  
  if (fix.valid.time&&fix.dateTime.hours>11){h=fix.dateTime.hours-12;Mdn=" PM";}else{h=fix.dateTime.hours;Mdn=" AM";}
  if (fix.valid.time&&fix.dateTime.hours==0||h==0) {h=12;}
  m=fix.dateTime.minutes;
  s=fix.dateTime.seconds;
 	if(h<10){Time[0] = ' ';}else	//omits hours only <10 leading zero, make efficient for solar power batteries..useless data
	Time[0]  = h / 10 +'0';  
	Time[1]  = h % 10 +'0';
	Time[3]  = m  / 10 +'0';
	Time[4]  = m  % 10 +'0';	
	Time[6]  = s  / 10 +'0';
	Time[7]  = s  % 10 +'0';
	P.print(Time + Mdn);
}
// end of code.
