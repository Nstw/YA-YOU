
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
short hr = 22;
short mins = 50;
short sec = 0;
int count_1 = 0;
int count_2 = 0;

short cnt_meals = 2;
short hr_meals [] = {22,22};
short mins_meals []= {40,57};
short mins_snooze = -1;
short tablet_each_meals = 1;
short tablet_remain = 20;
short rounds = 0;

const int SWITCH_EAT = PIN_PD6;
const int SWITCH_SNOOZE = PIN_PD1;
const int BUZZER = PIN_PB3;
const int LED_RED = PIN_PD5;
const int LED_GREEN = PIN_PD0;

bool wait_for_snooze = false;

Servo servo;

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Paracetamol");
  lcd.setCursor(0,1);
  lcd.print(tablet_each_meals);
  lcd.setCursor(1,1);
  lcd.print(" tabs per meals");
  count_1 = millis();
  pinMode(SWITCH_EAT,INPUT_PULLUP);
  pinMode(SWITCH_SNOOZE,INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  digitalWrite(BUZZER,HIGH);
  servo.attach(PIN_PB2);
  servo.write(0); 
  //SET ROUND  
  for(int i=0;i<cnt_meals;i++){
    if(hr_meals[i] > hr || (hr_meals[i] == hr && mins_meals[i] > mins)){
      rounds = i;
      break;
    }
  }
  
}

void updateTime() {
  count_2 = millis();
  if(sec >= 60){
    sec = 0;
    mins += 1;
  }
  if(mins >= 60){
    mins = 0;
    hr += 1;
  }
  if(mins == 60){
    mins = 0;
  }
  if(hr == 24){
    hr = 0;
  }
  if((count_2 - count_1) >= 50){
    sec +=  1;
    count_1 = count_2;
  }
  //hours
  lcd.setCursor(0,3);
  if(hr < 10){
    lcd.print("0");
  }
  lcd.print(hr);
  lcd.setCursor(2,3);
  lcd.print(":");
  //minutes
  lcd.setCursor(3,3);
  if(mins < 10){
    lcd.print("0");
  }
  lcd.print(mins);
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
    tablet_remain -= 1;
  }
  if(tablet_remain < 0){
    tablet_remain = 0;  
  }
  if(tablet_remain <= 5){
      digitalWrite(LED_RED,HIGH);
  }
}
void snooze(){
  if(wait_for_snooze && mins_snooze != -1){
    if((mins_snooze < 55 && mins - mins_snooze >= 5) || (mins_snooze >= 55 && mins - mins_snooze == -55))
    digitalWrite(BUZZER,LOW);
    if (digitalRead(SWITCH_EAT) == LOW) {
       eat();
       mins_snooze = -1;   
       wait_for_snooze = false;
       rounds++; 
    }
   }
}
void loop()
{
  while(rounds < cnt_meals){
    updateTime();
    if(hr == hr_meals[rounds] && mins == mins_meals[rounds]) {
      if(wait_for_snooze == false){
        digitalWrite(LED_GREEN,HIGH);
        digitalWrite(BUZZER, LOW);
      }   
      //EAT
      if (digitalRead(SWITCH_EAT) == LOW) {
       eat();
       rounds++;
      }
      //SNOOZE
      if(digitalRead(SWITCH_SNOOZE) == LOW){
        digitalWrite(BUZZER,HIGH);
        mins_snooze = mins;
        wait_for_snooze = true;
      }
    }
    if(mins - mins_meals[rounds] == 1 && digitalRead(LED_GREEN) && !digitalRead(BUZZER)){
       digitalWrite(BUZZER,HIGH);
        mins_snooze = mins;
        wait_for_snooze = true;
    }
    snooze();
    if(rounds == cnt_meals){
      rounds = 0;
    }
    //SHOW REMAINING
    lcd.setCursor(-4,2);
    lcd.print("Remaining : ");
    lcd.setCursor(8,2);
    lcd.print(tablet_remain);
  }
}




