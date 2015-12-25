#include "LCD5110_Basic.h"
LCD5110 myGLCD(8,9,10,11,12);
extern uint8_t SmallFont[];
void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
}
void loop()
{
  myGLCD.clrScr();
  //myGLCD.print("Upper case:", LEFT, 0);
  //myGLCD.print("ABCDEFGHIJKLM", CENTER, 16);
  //myGLCD.print("NOPQRSTUVWXYZ", CENTER, 24);
  //myGLCD.print("geeetech.com", CENTER, 40);
  myGLCD.print("Use Well",CENTER,24);
  delay (5000);
 
  /*myGLCD.clrScr();
  myGLCD.print("Lower case:", LEFT, 0);
  myGLCD.print("abcdefghijklm", CENTER, 16);
  myGLCD.print("nopqrstuvwxyz", CENTER, 24);
  myGLCD.print("geeetech.com", CENTER, 40);
  delay (5000);
 
  myGLCD.clrScr();
  myGLCD.print("Numbers:", LEFT, 0);
  myGLCD.print("0123456789", CENTER, 16);
  myGLCD.print("geeetech.com", CENTER, 40);
  delay (5000);
 
  myGLCD.clrScr();
  myGLCD.print("Special:", LEFT, 0);
  myGLCD.print("!\"#$%&'()*+,-.", CENTER, 16);
  myGLCD.print("/:;<=>?@[\\]^_`", CENTER, 24);
  myGLCD.print("{|}~", CENTER, 32);
  myGLCD.print("geeetech.com", CENTER, 40);
  delay (5000);*/
}
