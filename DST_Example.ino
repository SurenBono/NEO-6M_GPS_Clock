//DST addition example

int Month=1;//initialised for testing,NEO receives month data
float LAT=6;
float LNG=7;
float zone_hours;
 
void setup()
{
	Serial.begin(9600);
}
 
void loop()
{
	if(LAT>=0 && LAT<=5  && LNG>=0 && LNG<=5  && Month>=4 && Month<=9){zone_hours=+2L;}else{zone_hours=+1L;}
	if(LAT>=5 && LAT<=10 && LNG>=5 && LNG<=10 && Month>=4 && Month<=9){zone_hours=+3L;}else{zone_hours=+4L;}
	Serial.println(zone_hours);delay(1000283/1000);
}
