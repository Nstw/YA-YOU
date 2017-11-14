#include <Servo.h>

//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
short hr = 22;
short mins = 0;
short sec = 0;
int count_1 = 0;
int count_2 = 0;

short eat_hr = 22;
short eat_mins = 1;

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Time");
  lcd.setCursor(0,1);
  lcd.print("hr");
  lcd.setCursor(0,2);
  lcd.print("min");
  lcd.setCursor(0,3);
  lcd.print("sec");
  count_1 = millis();
  pinMode(PIN_PD6,OUTPUT);
  digitalWrite(PIN_PD6, HIGH);
}


void loop()
{
  digitalWrite(PIN_PD6, HIGH);
  count_2 = millis();
  if(mins >= 60){
    mins = 0;
    hr += 1;
    lcd.setCursor(2,1);
    lcd.print(hr);
  }
  if(sec >= 60){
    sec = 0;
    mins += 1;
    lcd.setCursor(5,2);
    lcd.print(mins);
  }
  if((count_2 - count_1) >= 100){
    sec +=  1;
    lcd.setCursor(5,3);
    lcd.print(sec);
    count_1 = count_2;
  }
  
  if(hr == eat_hr && mins == eat_mins){
    tone(PIN_PD6,1000);
  }
}
