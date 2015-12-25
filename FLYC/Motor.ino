/*
 * Find these functions in MWC Output sign.
 */
#define ESC_CALIB_CANNOT_FLY 0  //calibrate all attached ESCs
uint8_t PWM_PIN[8] = {9,10,11,3,6,5,A2,12};   //for a quad+: rear,right,left,front

void writeMotors() { // [1000;2000] => [125;250]
   /********  Specific PWM Timers & Registers for the atmega328P (Promini)   ************/
  OCR1A = ((motor[0]>>2) - 250);
  OCR1B = ((motor[1]>>2) - 250);
  OCR2A = ((motor[2]>>2) - 250);
  OCR2B = ((motor[3]>>2) - 250);
}
void writeAllMotors(int16_t mc) {   // Sends commands to all motors
  for (uint8_t i =0;i<4;i++) {
    motor[i]=mc;
  }
  writeMotors();
}
void initOutput() {
  for(uint8_t i=0;i<4;i++) {
    pinMode(PWM_PIN[i],OUTPUT);
  }
  TCCR1A |= _BV(COM1A1); // connect pin 9  to timer 1 channel A
  TCCR1A |= _BV(COM1B1); // connect pin 10 to timer 1 channel B
  TCCR2A |= _BV(COM2A1); // connect pin 11 to timer 2 channel A
  TCCR2A |= _BV(COM2B1); // connect pin 3  to timer 2 channel B
  
  /********  special version of MultiWii to calibrate all attached ESCs ************/
  #if defined(ESC_CALIB_CANNOT_FLY)
    writeAllMotors(ESC_CALIB_HIGH);   // default is 2000
    delay(3000);
    writeAllMotors(ESC_CALIB_LOW);    // same as MINCOMMAND, default is 1000
    delay(500);
    while (1) {
      delay(5000);
      blinkLED(2,20, 2);
    }
    exit; // statement never reached
  #endif
  writeAllMotors(MINCOMMAND);
  delay(300);
}

