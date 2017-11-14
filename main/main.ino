
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
short hr = 22;
short mins = 0;
short sec = 0;
int count_1 = 0;
int count_2 = 0;

short cnt_meals = 2;
short hr_meals [] = {22,22};
short mins_meals []= {1,10};
short mins_snooze = -1;
short tablet_each_meals = 2;
short cnt_tablet = 6;
short rounds = 0;

const int SWITCH_EAT = PIN_PB1;
const int SWITCH_SNOOZE = PIN_PB0;
const int BUZZER = PIN_PB3;
const int LED_RED = PIN_PC0;
const int LED_GREEN = PIN_PC1;

bool firstEat = true;
bool Eaten = false;
bool press_for_snooze = false;

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
  pinMode(SWITCH_EAT,INPUT_PULLUP);
  pinMode(SWITCH_SNOOZE,INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  digitalWrite(BUZZER,HIGH);
  servo.attach(PIN_PB2);
  servo.write(0); 
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
void eat()
{
  int i=1;
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(BUZZER,HIGH);
  for(;i<=tablet_each_meals;i++){        
    servo.write(200); 
    delay(1000);      
    servo.write(0); 
    delay(1000);
    cnt_tablet -= 1;
  }  
}
void snooze(){
  if(press_for_snooze && mins-mins_snooze >= 5){
    digitalWrite(BUZZER,LOW);
    if (digitalRead(SWITCH_EAT) == LOW) {
       eat();
       mins_snooze = -1;   
       press_for_snooze = false;
       rounds++; 
 //      cnt_tablet -= tablet_each_meals;
    }
   }
}
void loop()
{
  while(rounds < cnt_meals){
    updateTime();
    //digitalWrite(LED_RED,HIGH);
    if(hr == hr_meals[rounds] && mins == mins_meals[rounds]) {
      if(!press_for_snooze){
        digitalWrite(LED_GREEN,HIGH);
        digitalWrite(BUZZER, LOW);
      }   
      //eat
      if (digitalRead(SWITCH_EAT) == LOW) {
       eat();
       rounds++;
     //  cnt_tablet -= tablet_each_meals;
      }
      //snooze
      if(digitalRead(SWITCH_SNOOZE) == LOW){
        digitalWrite(BUZZER,HIGH);
        mins_snooze = mins;
        press_for_snooze = true;
      }
    }
    snooze();
    if(cnt_tablet <= 5){
      digitalWrite(LED_RED,HIGH);
    }
  }
}




