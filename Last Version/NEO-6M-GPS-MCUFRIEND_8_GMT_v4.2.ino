/*
MCUFRIEND TFT + NEO-6M + SMD UNO + UNO SCREW SHIELD = SOMEWHAT CHEAPEST/SIMPLE STRATUM 1  NON-DST +8 PRECISION GPS CLOCK
Devoloped by http://srotogargees.business.site/

 ** Version 2 Bugfix **
 
 - 12-H AM/PM version
 - added leading zero to min & seconds ,update every seconds.
 - Note: Decoded Coordinate,Altitude,Speed & Satellites count might not be as accurate as the prioritized CLOCK display.
 - PRECISION = +/- 1 Second on lock vs MCU processing speed + display latency.
 - Clock Accuracy compared to NTP = +/- 500 Millis - 1 Second.
 - Allow few minutes,place GPS near windows for fast sat lock.
 - Non-DST +8 GMT/UTC Timezone covers 4 Billion population.
 - Irkutsk,Shanghai,Beijing,Taipei,Hong Kong,Manila,Kuala Lumpur,Singapore,Makassar & Perth.
 - NEO_TX->PIN 10((UNOSS*RX)only rec),NEO_RX->UNOSS*TX can be ignored(no transmit to sat).
 - or find a software serial pin that will not conflict with display pins.
 - SS*=Software Serial ,pins that mimics MCU RX&TX to counter upload failure if RX/TX were utilised.
 - Feel free to mod according to your pref(opensourced).STM MCU,OLED,8-Seg,LedMatrix,Serial Monitor...etc
 - Simple/Fast 150 line Hobbist project.

*/

#include <Adafruit_GFX.h>  
#include <MCUFRIEND_kbv.h>  
#include <NeoSWSerial.h>
#include <NMEAGPS.h>

MCUFRIEND_kbv tft;

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

NMEAGPS gps;  
gps_fix fix;  
uint8_t fixCount;
NeoSWSerial serialgps(10,0);	//NEOTX->UNOSS*RX,NEORX<-UNOSS*TX

int h;  	//  Global localised variable declaration for 12-hour AM/PM format
String M;	//  Variable Meridian AM/PM

const int32_t zone_hours= +8L; 	// Timezone for non-Daylight Saving zone

void setup()
{
 tft.reset(); 
 uint16_t identifier = tft.readID();
 //Serial.print("ID = 0x");
 //Serial.println(identifier, HEX);
 if (identifier == 0xEFEF) identifier = 0x9486;
 tft.begin(identifier);  
 serialgps.begin(9600); 
 tft.setTextColor(GREEN); 
 tft.setRotation(3); 				//Portrait(0=4),Landscape(1=5).Portrait Reversed 2=6
 tft.setCursor(50, 65);
 tft.setTextSize(3);
 tft.print("S&G GPS TIME");			//mod to your pref title/logo.
}

void adjustTime( NeoGPS::time_t & dt )
{
  NeoGPS::clock_t seconds = dt;     // convert date/time structure to seconds
  const NeoGPS::clock_t  zone_offset = zone_hours * NeoGPS::SECONDS_PER_HOUR;
  seconds += zone_offset;
  dt = seconds;                     // convert seconds structure back to date/time
} 

void loop(){//set fixCount >=1 if loc ,alt,sat detail didnt show or just wait
if (gps.available( serialgps )) {fix = gps.read();fixCount++;
if (fixCount >= 1){fixCount = 0; if (fix.valid.time)adjustTime(fix.dateTime);printGPSdata();}}}

void printGPSdata()
{ 
  tft.fillScreen(BLACK);        		//clearscreen function also counters tft screenburns
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.print("= Sroto&Gargees =");       //mod to your pref title/logo.
  tft.println();
  tft.println();
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(18, 46);
  tft.print(F("LOC : "));
  if (fix.valid.location) {
    tft.setTextColor(GREEN);
    tft.print( fix.latitude(), 5 ); 
    tft.print( ", " );
    tft.print( fix.longitude(), 5 );
    tft.println();
  }
  tft.println();
  tft.setTextColor(WHITE);
  tft.setCursor(40, 71);
  tft.print( F(" Date : ") );
  if (fix.valid.date) {
    tft.setTextColor(GREEN);
    tft.print( fix.dateTime.date  ); tft.setTextColor(WHITE);tft.print('.'); tft.setTextColor(GREEN);
    tft.print( fix.dateTime.month ); tft.setTextColor(WHITE);tft.print('.');tft.setTextColor(GREEN);
    tft.print( fix.dateTime.year  );
    tft.println();}
  tft.setCursor(18, 100);
  tft.setTextSize(4);
  tft.setTextColor(YELLOW);
  tft.print(" ");
  if (fix.valid.time&&fix.dateTime.hours>11){h=fix.dateTime.hours-12;M=" PM";}else{h=fix.dateTime.hours;M=" AM";}
  if (fix.valid.time&&fix.dateTime.hours==0||h==0) {h=12;}
  if (h<10){tft.print(" ");}tft.print(h);
  tft.setTextSize(4);tft.setTextColor(WHITE);
  tft.print(':'); tft.setTextColor(YELLOW);
  if (fix.dateTime.minutes<10){tft.print("0");tft.print(fix.dateTime.minutes);}else{tft.print(fix.dateTime.minutes);}
  tft.setTextColor(WHITE);tft.print(':');tft.setTextColor(YELLOW);tft.setTextSize(3);
  if (fix.dateTime.seconds<10){tft.print("0");tft.print(fix.dateTime.seconds);}else{tft.print(fix.dateTime.seconds);}
  tft.print(M);
  tft.setTextSize(2);
  tft.setCursor(36, 143);
  tft.setTextColor(WHITE);
  tft.print( "Altitude (m) : " );
  if (fix.valid.altitude);
  tft.setTextColor(GREEN);
  tft.print( fix.altitude() );
  tft.println();
  tft.setTextColor(WHITE);
  tft.setCursor(36, 169);
  tft.print( "Speed (km/h) : " );
  if (fix.valid.speed);
  tft.setTextColor(GREEN);
  tft.print( fix.speed_kph() );
  tft.println();
  tft.println();
  tft.setCursor(36, 194);
  tft.setTextColor(WHITE);
  tft.print( "Satellites   : " );
  if (fix.valid.satellites);
  tft.setTextColor(GREEN);
  tft.println( fix.satellites );
  tft.setCursor(36, 218);
  tft.setTextColor(WHITE);
  tft.print("Timezone     : ");
  tft.setTextColor(GREEN);
  tft.print(zone_hours);
  tft.print(" UTC");
}
// end of code.
