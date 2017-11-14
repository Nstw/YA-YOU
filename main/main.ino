#include <Servo.h>
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
short mins_snooze = -1;

const int SWITCH_EAT = PIN_PB1;
const int SWITCH_SNOOZE = PIN_PB0;
//const int SERVO = PIN_PB2;
const int BUZZER = PIN_PB3;
const int LED_RED = PIN_PC0;
const int LED_GREEN = PIN_PC1;
//const int SERVO = PIN_PB2;

bool firstEat = true;
bool Eaten = false;

Servo servo;

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
/*  lcd.backlight();
  lcd.setCursor(0,0);
//  lcd.print("Time");
//  lcd.setCursor(0,1);
//  lcd.print("hr");
//  lcd.setCursor(0,2);
  lcd.print("min");
  lcd.setCursor(0,3);
  lcd.print("sec");*/
  count_1 = millis();
//  pinMode(PIN_PD6,OUTPUT);
//  digitalWrite(PIN_PD6, HIGH);
//  servo.attach(PIN_PC1);
  pinMode(SWITCH_EAT,INPUT_PULLUP);
  pinMode(SWITCH_SNOOZE,INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
//  pinMode(SERVO,OUTPUT);
  digitalWrite(BUZZER,HIGH);
  servo.attach(PIN_PB2);
  servo.writeMicroseconds(1500);
  servo.write(0); 
  delay(1000);
}

void updateTime() {
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
}

void loop()
{
  updateTime();
  if(hr == eat_hr && mins == eat_mins && !Eaten) {
    if (firstEat) {
      firstEat = false;
      digitalWrite(LED_GREEN,HIGH);
//      delay(500);
      digitalWrite(BUZZER, LOW);
//      delay(500);
    }   
    //eat
    if (digitalRead(SWITCH_EAT) == LOW) {
      digitalWrite(LED_GREEN,LOW);
      digitalWrite(BUZZER,HIGH);        
       servo.write(200); 
       delay(1000);      
       servo.write(0); 
       delay(1000);
       Eaten = true;    
    }
    //snooze
    if(digitalRead(SWITCH_SNOOZE) == LOW){
      digitalWrite(BUZZER,HIGH);
      mins_snooze = mins;
      }
    }
   if(mins - mins_snooze >= 5){
      mins_snooze = 0;
      digitalWrite(BUZZER,LOW);
      if (digitalRead(SWITCH_EAT) == LOW) {
        digitalWrite(LED_GREEN,LOW);
        digitalWrite(BUZZER,HIGH);        
        servo.write(200); 
        delay(1000);      
        servo.write(0); 
        delay(1000);
        Eaten = true;    
      }
}
}


