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
NeoSWSerial serialgps(0,3);


void setup()
{
 tft.reset(); 
    uint16_t identifier = tft.readID();
    Serial.print("ID = 0x");
    Serial.println(identifier, HEX);
    if (identifier == 0xEFEF) identifier = 0x9486;
    tft.begin(identifier);
 Serial.begin(19200);  
 serialgps.begin(9600); 
 tft.setTextColor(GREEN); 
 tft.setRotation(3); //Portrait(0=4),Landscape(1=5).Portrait Reversed 2=6
 tft.setCursor(50, 65);
 tft.setTextSize(3);
 tft.print("S&G GPS TIME");
 Serial.println( F("Sroto&Gargees") );

}

void loop()
{
  
 // Check for GPS characters and parse them
 if (gps.available( serialgps )) 
 {
  // Once per second, a complete GPS fix structure is ready.  Get it.
  fix = gps.read();

  // Count elapsed seconds
  fixCount++;

  if (fixCount >= 10)
  {
    fixCount = 0; // reset counter

    // adjust from UTC to local time
    if (fix.valid.time)
      adjustTime( fix.dateTime );

    printGPSdata();
  }
 }
}


void printGPSdata()
{ 
  tft.fillScreen(BLACK); 
  tft.setCursor(48, 10);
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.print("Sroto&Gargees");
  tft.println();
  Serial.print( F("Latitude / Longitude : ") ); 
  tft.println();
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(18, 50);
  tft.print(F("LOC : "));
  if (fix.valid.location) {
    Serial.print( fix.latitude(), 5 ); 
    tft.setTextColor(GREEN);
    tft.print( fix.latitude(), 5 ); 
    Serial.print( F(", ") ); 
    tft.print( ", " );
    Serial.print( fix.longitude(), 5 );
    tft.print( fix.longitude(), 5 );
    tft.println();
  }
  Serial.println();
  tft.println();

  Serial.print( F("Date: ") );
  tft.setTextColor(WHITE);
  tft.setCursor(18, 75);
  tft.print( F("Date: ") );
  if (fix.valid.date) {
    Serial.print( fix.dateTime.date  ); Serial.print('/'); 
    Serial.print( fix.dateTime.month ); Serial.print('/');
    Serial.print( fix.dateTime.year  );
    tft.setTextColor(GREEN);
    tft.print( fix.dateTime.date  ); tft.print('/'); 
    tft.print( fix.dateTime.month ); tft.print('/');
    tft.print( fix.dateTime.year  );
    tft.println();
    
  }

  Serial.print("  Time: " );
  tft.setCursor(18, 105);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print( "T: " );
  if (fix.valid.time) {
    Serial.print( fix.dateTime.hours ); Serial.print(':');
    tft.setTextColor(WHITE);
    tft.setTextSize(4);
    tft.print( fix.dateTime.hours ); tft.print(':');  
    if (fix.dateTime.minutes < 10)
      Serial.print( '0' );
      
    Serial.print(fix.dateTime.minutes); Serial.print(':');
    tft.print(fix.dateTime.minutes); tft.print(':');
    if (fix.dateTime.seconds < 10)
      Serial.print( '0' );
    
    Serial.print( fix.dateTime.seconds ); 
    tft.print( fix.dateTime.seconds );
    Serial.print('.');
    //tft.print('.');
    if (fix.dateTime_cs < 10)
      Serial.print( '0' );
      
    Serial.print( fix.dateTime_cs );
    //tft.print( fix.dateTime_cs );
    
  }
  Serial.println();
  
 
  tft.setTextSize(2);
  Serial.print( F("Altitude (meters): ") );
  tft.setCursor(18, 150);
  tft.setTextColor(WHITE);
  tft.print( "Alt(m)      : " );
  if (fix.valid.altitude)
    Serial.print( fix.altitude() );
    tft.setTextColor(GREEN);
    tft.print( fix.altitude() );
  Serial.println();
  tft.println();

  Serial.print( F("Speed (kmph): ") );
  tft.setTextColor(WHITE);
  tft.setCursor(18, 175);
  tft.print( "Speed (km/h): " );
  if (fix.valid.speed)
    Serial.print( fix.speed_kph() );
    tft.setTextColor(GREEN);
    tft.print( fix.speed_kph() );
  Serial.println();
  tft.println();
  tft.println();
  tft.setCursor(18, 200);
  Serial.print( F("Satellites : ") );
  tft.setTextColor(WHITE);
  tft.print( "Satellites  : " );
  if (fix.valid.satellites)
    Serial.println( fix.satellites );
    tft.setTextColor(GREEN);
    tft.println( fix.satellites );
  Serial.println();
  Serial.println();
  
}


//--------------------------

void adjustTime( NeoGPS::time_t & dt )
{
  NeoGPS::clock_t seconds = dt; // convert date/time structure to seconds

  // Offset to the local time
  const int32_t          zone_hours  = +8L;
  const NeoGPS::clock_t  zone_offset = zone_hours * NeoGPS::SECONDS_PER_HOUR;
  seconds += zone_offset;

  dt = seconds; // convert seconds back to a date/time structure

} // adjustTime
