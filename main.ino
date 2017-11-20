#include <SPI.h>
#include <LiquidCrystal.h>
#define motion1 6
#define motion2 7
#define b1 2
#define b2 3
#define b3 4
#define b4 5
#define alarmOn 1//motor or led
#define buzzer 4
LiquidCrystal lcd(10);

int seconds=0,minutes=0,hours=0,days=1,months=1,years=2015,almSec=0,almMin=0,almHr=0,leapYear;
bool alarmIsOn;

//LCD brightness
void wait(int milsec){
  unsigned long long stopTime=millis()+milsec;
  while(stopTime>millis())
  {}
}

void alarm(){
  int frequency=7;
  unsigned long long stopTimer=0,stopTime=millis()+1800000;
  boolean bedArea=true,roomArea=false,onBed=true,timer=false;
  while(stopTime>millis()){
    if(digitalRead(motion1)){
      bedArea=true;
      roomArea=false;
      stopTimer=millis()+180000;
    }
    if(digitalRead(motion2)){
      bedArea=false;
      roomArea=true;
      timer=true;
    }
    if(stopTimer>millis())
      timer=false;
    else
      timer=true;
    if(bedArea&&!timer){
      tone(buzzer, frequency*500, 20);
      stopTime=millis()+1800000;
      if(frequency==10)
        frequency=7;
    }
  }
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

void timeChange(){
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
  pinMode(motion1, INPUT);
  pinMode(motion2, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  pinMode(buzzer, OUTPUT);
  control(2);
  changeDate(2);    
}

void rightTime(int com){
  unsigned long long add=millis();
  switch(com){
    case 1: alarm();break;
    case 2: changeDate(2);break;
    case 3: control(1);;break;
    case 4: control(2);;break;
  }
  for(int i=0;i<int((millis()-add)/1000);i++)
    timeChange();
}

void loop() {
  if(digitalRead(b1))
    rightTime(2);
  if(digitalRead(b2))
    alarmIsOn=!alarmIsOn;
  if(digitalRead(b3))
    rightTime(3);
  if(digitalRead(b4))
    rightTime(4);
  if(!millis()%1000){
    timeChange();
    lcd.setCursor(4,0);
    lcd.print((hours<10?'0'+String(hours):String(hours))+':'+(minutes<10?'0'+String(minutes):String(minutes))+':'+
                              (seconds<10?'0'+String(seconds):String(seconds)));
    if(alarmIsOn&&seconds==almSec&&minutes==almMin&&hours==almHr)
      rightTime(1);
  }
}

//// The LCD is usually interfaced via 16 pins which are labelled as shown below:
//                         //Connections to Arduino
//                         //  LCD                                                 Connection
//                         //  1. GND - Ground                                     GND
//                         //  2. VDD - 3 - 5V                                     5V
//                         //  3. VO  - Contrast                                   (Tap off a 5K - 10K pot across VCC and Ground)
//#define LCD_RS         8 //  4. RS  - Register Select - 0=Command / 1=Character  Arduino Pin as defined
//                         //  5. RW  - Read/Write - 0=Write or 1=Read             GND
//#define LCD_ENABLE     9 //  6. E   - Enable - Enable data transmit              Arduino Pin as defined
//                         //  7. DB0 - Data Bit 0                                 N/A
//                         //  8. DB1 - Data Bit 1                                 N/A
//                         //  9. DB2 - Data Bit 2                                 N/A
//                         // 10. DB3 - Data Bit 3                                 N/A
//#define LCD_DB4        4 // 11. DB4 - Data Bit 4 - used in 4 bit operation       Arduino Pin as defined
//#define LCD_DB5        5 // 12. DB5 - Data Bit 5 - used in 4 bit operation       Arduino Pin as defined
//#define LCD_DB6        6 // 13. DB6 - Data Bit 6 - used in 4 bit operation       Arduino Pin as defined
//#define LCD_DB7        7 // 14. DB7 - Data Bit 7 - used in 4 bit operation       Arduino Pin as defined
//#define LCD_Backlight 10 // 15. BL1 - Backlight +                                Emitter of 2N3904, Collector to VCC, Base to D10 via 10K resistor
//                         // 16. BL2 - Backlight -                                GND
//
//// include the library code:
//#include <LiquidCrystal.h>
//
//// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
//
//void setup() {
//  // set up the LCD's number of columns and rows:
//  lcd.begin(20, 4);
//  lcd.clear();
//  pinMode(LCD_Backlight, OUTPUT);
//  analogWrite(LCD_Backlight, 128); // Set the brightness of the backlight
//}
