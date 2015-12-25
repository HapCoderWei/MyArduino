#include <Servo.h>
Servo servo;
int val;
void setup() {
  // put your setup code here, to run once:
  servo.attach(6);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("S-Y: ");
  val = analogRead(0);
  Serial.println(val);
  val = 0.001 * val * val;
  val = map(val, 0, 1023, 0, 179);
  servo.write(val);
  delay(10);
}
