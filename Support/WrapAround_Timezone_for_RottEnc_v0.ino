
int count;
float UTC;


void setup()
//Sample simple Rotary Encoder Timezone Wraping Arounds

{
	Serial.begin(9600);
	count=35;
}

void loop()
{
	count=count-1;
	
	//if(count==0){UTC=-12;}
	if(count==1){UTC=-12;}
	if(count==2){UTC=-11;}
	if(count==3){UTC=-10;}
	if(count==4){UTC=-9.5;}
	if(count==5){UTC=-9;}
	if(count==6){UTC=-8;}
	if(count==7){UTC=-7;}
	if(count==8){UTC=-6;}
	if(count==9){UTC=-5;}
	if(count==10){UTC=-4;}
	if(count==11){UTC=-3.5;}
	if(count==12){UTC=-3;}
	if(count==13){UTC=-2;}
	if(count==14){UTC=-1;}
	if(count==15){UTC=0;}
	if(count==16){UTC=1;}
	if(count==17){UTC=2;}
	if(count==18){UTC=3;}
	if(count==19){UTC=3.5;}
	if(count==20){UTC=4;}
	if(count==21){UTC=4.5;}
	if(count==22){UTC=5;}
	if(count==23){UTC=5.5;}
	if(count==24){UTC=5.75;}
	if(count==25){UTC=6;}
	if(count==26){UTC=6.5;}
	if(count==27){UTC=7;}
	if(count==28){UTC=8;}
	if(count==29){UTC=8.75;}
	if(count==30){UTC=9;}
	if(count==31){UTC=9.5;}
	if(count==32){UTC=10;}
	if(count==33){UTC=10.5;}
	if(count==34){UTC=11;}
	if(count==35){UTC=12;}
	if(count==36){UTC=12.75;}
	if(count==37){UTC=13;}
	if(count==38){UTC=14;}
	
	if(count>38){count=1;UTC=-12;}
	if(count<1) {count=38;UTC=14;}
	Serial.print("counter :");Serial.print(count);Serial.print("    UTC : ");Serial.print(UTC);
	
	Serial.println();
	delay(500);
}
