short hr = 22;
short mins = 0;
short sec = 0;
int count_1 = 0;
int count_2 = 0;

short eat_hr = 22;
short eat_mins = 1;
void setup() {
  // put your setup code here, to run once:
  count_1 = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
   count_2 = millis();
  if(mins >= 60){
    mins = 0;
    hr += 1;
  }
  if(sec >= 60){
    sec = 0;
    mins += 1;
  }
  if((count_2 - count_1) >= 100){
    sec +=  1;
    count_1 = count_2;
  }
}
