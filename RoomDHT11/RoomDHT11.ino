#include "LCD5110_Basic.h"
#include <dht.h>

#define dht_dpin 10  // define dht data pin is 10
// myGLCD(Clk, Din, DC, RST, CE)
LCD5110 myGLCD(5,6,7,9,8);
extern uint8_t SmallFont[];
dht DHT; 
String hum;
String tem;

void setup() {
  // put your setup code here, to run once:
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
}

void loop() {
  // put your main code here, to run repeatedly:
  //myGLCD.clrScr();
  DHT.read11(dht_dpin);  // read data from dht11
  static char h[7];
  static char t[7];
  dtostrf(DHT.humidity, 2, 0, h);   // convert double type to char[]
  dtostrf(DHT.temperature, 3, 1, t);  // convert double type to char[]
  hum="Humidity: ";
  hum=hum+h+"%";
  tem="Temper: ";
  tem=tem + t+"C";
  myGLCD.print(hum, CENTER, 16);
  myGLCD.print(tem, CENTER, 24);
  delay(1000);
}
