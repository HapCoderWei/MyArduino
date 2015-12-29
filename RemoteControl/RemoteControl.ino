#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THRO_PIN  1
#define YAW_PIN   2
#define PITCH_PIN 3
#define ROLL_PIN  4

int T_temp, P_temp, R_temp, Y_temp, i;
int Throttle, Pitch, Roll, Yaw;

RF24 radio(7, 8);
const byte rxAddr[6] = "00001";
unsigned char TxBuf[32] = {0};

void setup(){
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();
  Serial.begin(9600);
}
void loop(){
  T_temp = 0;  Y_temp = 0;
  P_temp = 0;  R_temp = 0;
  for(i = 0; i < 3; i++) {
    T_temp += analogRead(THRO_PIN);
    Y_temp += analogRead(YAW_PIN);
    P_temp += analogRead(PITCH_PIN);
    R_temp += analogRead(ROLL_PIN);
  }
  T_temp /= 3;  Y_temp /= 3;
  P_temp /= 3;  R_temp /= 3;
  
  Throttle = map(T_temp, 0, 1023,    0, 1000);
  Yaw      = map(Y_temp, 0, 1023, -100, 100);
  Pitch    = map(P_temp, 0, 1023, -100, 100);
  Roll     = map(R_temp, 0, 1023, -100, 100);
  
  Serial.println(Throttle);
  TxBuf[0] = Throttle;
  TxBuf[1] = Yaw;
  TxBuf[2] = Pitch;
  TxBuf[3] = Roll;

  TxBuf[31] = 0x5A;

  radio.write(&TxBuf, sizeof(TxBuf));
}

