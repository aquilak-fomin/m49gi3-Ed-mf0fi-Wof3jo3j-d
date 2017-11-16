#include <LiquidCrystal.h>
#define motion1 6
#define motion2 7
#define b1 2
#define b2 3
#define b3 4
#define b4 5
#define alarmOn 5//motor or led
#define buzzer 4
LiquidCrystal lcd(13,12,11,10,9,8);

//unsigned long long timeNow,startTime,startDate;
int seconds=0,minutes=0,hours=0,days=1,months=1,years=2015,almSec=0,almMin=0,almHr=0,leapYear,alSec,alMin,alHrs;
char row1, row2, empty="                ";
//char row1[],row2[];

//LCD brightness
void wait(int milsec){
  unsigned long long stopTime=millis()+milsec;
  while(stopTime>millis())
  {}
}

void alarm(){
  
}

void control(int code){
  lcd.setCursor(0,0);
  code==1?lcd.print("Set alarm:"):lcd.print("Set time:");
  lcd.setCursor(0,1);
  switch(code){
    case 1:lcd.print((almHr<10?'0'+String(almHr):String(almHr))+':'+(almMin<10?'0'+String(almMin):String(almMin))+':'+
                              (almSec<10?'0'+String(almSec):String(almSec)));break;      
    case 2:lcd.print((hours<10?'0'+String(hours):String(hours))+':'+(minutes<10?'0'+String(minutes):String(minutes))+':'+
                              (seconds<10?'0'+String(seconds):String(seconds)));break;
  }
  while(!digitalRead(b4)){
    if (digitalRead(b1))
    {
      switch(code){
        case 1:almSec+=1;if(almSec==60)almSec=0;break;
        case 2:seconds+=1;if(seconds==60)seconds=0;break;
      }
    }
    if (digitalRead(b2))
    {
      minutes+=1;
      if (minutes==60){
        minutes=0;
        hours+=1;
      }
    }
    if (digitalRead(b3))
    {
      hours+=1;
      if (hours==24)
        hours=0;
    }
    if(digitalRead(b1)||digitalRead(b2)||digitalRead(b3)){
      lcd.setCursor(0,1);
      switch(code){
      case 1:lcd.print((almHr<10?'0'+String(almHr):String(almHr))+':'+(almMin<10?'0'+String(almMin):String(almMin))+':'+
                              (almSec<10?'0'+String(almSec):String(almSec)));break;      
      case 2:lcd.print((hours<10?'0'+String(hours):String(hours))+':'+(minutes<10?'0'+String(minutes):String(minutes))+':'+
                              (seconds<10?'0'+String(seconds):String(seconds)));break;
      }
      wait(200);
    }
  }
}

void changeDate(int code){
  int daysInMonth;
  if(code==1){//usual date change
    days++;
    if(!(months==2&&!(!years%400||(!years%4&&years%100))))
      daysInMonth=28+(months+int(months/8))%2+2%months+2*int(1/months);
    else
      daysInMonth=29;
    if(days>daysInMonth){
      days=1;
      months++;
      if(months==13){
        months=1;
        years++;
      }
    }
    lcd.setCursor(2,1);
    lcd.print((days<10?'0'+String(days):String(days))+'/'+(months<10?'0'+String(months):String(months))+'/'+String(years-2000));
    lcd.setCursor(10,1);
    switch((7000+days+(years-(14-months)/12)+int((years-(14-months)/12)/4)-(years-(14-months)/12)/100+(years-(14-months)/12)/400+(31*months+12*((14-months)/12)-2)/12)%7){
      case 1: lcd.print("MON");break;
      case 2: lcd.print("TUE");break;
      case 3: lcd.print("WED");break;
      case 4: lcd.print("THU");break;
      case 5: lcd.print("FRI");break;
      case 6: lcd.print("SAT");break;
      case 7: lcd.print("SUN");break;
    }
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set year:");    
    lcd.setCursor(0,1);
    lcd.print(years);
    while(!digitalRead(b4))
      if(digitalRead(b1) || digitalRead(b2)){
        !digitalRead(b1)?years+=1:years-=1;
        wait(200);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set year:");
        lcd.setCursor(0,1);
        lcd.print(years);
      }
    wait(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set month:");
    lcd.setCursor(0,1);
    lcd.print(months);
    while(!digitalRead(b4))
      if(digitalRead(b1) || digitalRead(b2)){
        !digitalRead(b1)?months+=1:months-=1;
        wait(200);
        if(months==13)
          months=1;
        if(months==-1)
          months=12;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set month:");
        lcd.setCursor(0,1);
        lcd.print(months);
      }
    wait(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set day:");
    lcd.setCursor(0,1);
    lcd.print(days);
    if(!(months==2&&!(!years%400||(!years%4&&years%100))))
      daysInMonth=28+(months+int(months/8))%2+2%months+2*int(1/months);
    else
      daysInMonth=29;
    while(!digitalRead(b4))
      if(digitalRead(b1) || digitalRead(b2)){
        !digitalRead(b1)?days+=1:days-=1;
        wait(200);
        if(days==daysInMonth+1)
          days=1;
        if(days==0)
          days=daysInMonth;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Set day:");
        lcd.setCursor(0,1);
        lcd.print(days);
      }
    leapYear=0;
    for(int i=2015;i<months==1?years:years+1;i++)
      if(!years%400||(!years%4&&years%100))
        leapYear++;
    int daysInThisYear=0;
    for(int i=1;i!=months;i++)
      daysInThisYear+=28+(i+int(i/8))%2+2%i+2*int(1/i);
    //startDate=(days+daysInThisYear+365*(years-2015)+leapYear)*86400;
  }
}

void setup() {
  lcd.begin(16,2);
  //Serial.begin(9600);
  pinMode(motion1, INPUT);
  pinMode(motion2, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  control(2);
  changeDate(2);    
}

void loop() {
  //timeNow=startTime+startDate+millis()/1000;
  if(!millis()%1000){
    seconds++;
    if(seconds>59){
      seconds=0;
      minutes++;
      if(minutes>59){
        minutes=0;
        hours++;
        if(hours>23){
          hours=0;
          changeDate(1);
        }
      }
    }
    lcd.setCursor(4,0);
    lcd.print((hours<10?'0'+String(hours):String(hours))+':'+(minutes<10?'0'+String(minutes):String(minutes))+':'+
                              (seconds<10?'0'+String(seconds):String(seconds)));
    if(seconds==alSec&&minutes==alMin&&hours==alHrs)
      alarm();
  }

//  Serial.print("\n");
//  if (digitalRead(motion1) == HIGH) {
//      lcd.print("Motion detected!");
//  }
}


//    startDate=(days+daysInThisYear+365*(years-2015)+leapYear)*86400;


/*
    while(b4!=HIGH){
    if (b1==HIGH)
    {
      wait(100);
      seconds+=1;
      if (seconds==60){
        seconds=0;
        minutes+=1;
      }
    }
    if (b2==HIGH)
    {
      wait(100);
      minutes+=1;
      if (minutes==60){
        minutes=0;
        hours+=1;
      }
    }
    if (b3==HIGH)
    {
      wait(100);
      hours+=1;
      if (hours==24)
        hours=0;
    }
  }
  wait(1000);
  while(b4!=HIGH){
    if (b1==HIGH)
    {
      wait(100);
      seconds+=1;
      if (seconds==60){
        seconds=0;
        minutes+=1;
      }
    }
    if (b2==HIGH)
    {
      wait(100);
      minutes+=1;
      if (minutes==60){
        minutes=0;
        hours+=1;
      }
    }
    if (b3==HIGH)
    {
      wait(100);
      hours+=1;
      if (hours==24)
        hours=0;
    }
  }
 */
