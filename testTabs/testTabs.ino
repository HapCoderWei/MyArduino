#include "deh.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  i = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  myPrint();
  Serial.println(i);
}
