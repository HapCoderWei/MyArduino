#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THRO_PIN  2
#define YAW_PIN   3
#define PITCH_PIN 1
#define ROLL_PIN  0
#define Button    9

int T_temp, P_temp, R_temp, Y_temp, i;
int Throttle, Pitch, Roll, Yaw;
boolean HALT = false;  // halt flag

RF24 radio(7, 8);
const byte rxAddr[6] = "00001";
unsigned int TxBuf[32] = {0};

void setup(){
  //Serial.begin(115200);
  pinMode(Button,INPUT); //按钮端子作为输入
  
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();
  
  T_temp = 100;  P_temp = 100;
  while(T_temp > 50 || P_temp > 50) {
    T_temp = analogRead(THRO_PIN);
    P_temp = analogRead(PITCH_PIN);
//    Serial.print(T_temp);
//    Serial.print("\t");
//    Serial.println(P_temp);
  } // To Unlock: Thro minimal and Roll minimal
  
  TxBuf[0] = 1050;  // Step By 10, So [0, 100] -> [0, 1000]
  TxBuf[1] = 0;  TxBuf[2] = 0;  
  TxBuf[3] = 0;  TxBuf[4] = 0x5A;
  radio.write(&TxBuf, sizeof(TxBuf));
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
  
  Throttle = map(T_temp, 0, 1022,    105, 200);
  Yaw        = map(Y_temp, 0, 1022, -100, 100);
  Pitch      = map(P_temp, 0, 1022, -100, 100);
  Roll        = map(R_temp, 0, 1022, -100, 100);
  
  TxBuf[0] = Throttle * 10;  // Step By 10, So [0, 100] -> [0, 1000]
  TxBuf[1] = Yaw;
  TxBuf[2] = Pitch;
  TxBuf[3] = Roll;

  if(digitalRead(Button) == HIGH) {
    delay(10);
    if(digitalRead(Button) == HIGH) {
      //TxBuf[4] = 0;  // Halt!
      HALT = true;
    }
  }  
  else {
      //TxBuf[4] = 0x5A;  // Enable Aircraft
      //HALT = 0;
  }
  TxBuf[4] = (HALT == true)?0:0x5A;
  //Serial.println(Throttle);
  radio.write(&TxBuf, sizeof(TxBuf));
  
  //Serial.println(TxBuf[4]);
}

