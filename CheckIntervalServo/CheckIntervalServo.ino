#include <Servo.h>
Servo s;
bool f;
int t, o;
void setup() {
  // put your setup code here, to run once:
  s.attach(10);
  pinMode(8, INPUT);
  Serial.begin(9600);
  s.write(0);
  f = false;
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  int d;
  d = pulseIn(8, HIGH);
  if(d > 500 && d < 550) {
    if(!f) {
      f = true;  
      t = millis();  
      delay(2);  
    }
    else {
      o = millis()- t;
      Serial.println(o, DEC);
      while(1);
    }
  }
}
