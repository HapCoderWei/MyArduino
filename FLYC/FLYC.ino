/************ MPU6050 *****************/
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "mpu_def.h"
/************ NRF24l01 ****************/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "rf24_def.h"
/************* PID Define *************/
#include "pid.h"
/*************    Motor   *************/
#include <Servo.h>
#include "motor.h"
/*********** end of header ************/
#define LOOP_TIME  10000  // 10000 us = 10ms one cycle time
#define g     9.8f

unsigned long startLoop = 0;
unsigned long loopTime = 1;
unsigned int freq = 0;

void setup() {
  Serial.begin(115200);
  
  mpuSet();
  nrfSet();
  motorSet();
  PIDSet();

  //calibrate_sensors();
  delay(1500);
}
void loop() {
  // this code is for test how many times this scratch can run in a second
  // result is 100. Just So So... 
  /*******************************************************************/
  startLoop = micros();
  freq = 1000000/loopTime;
  //if(freq > 200 )  
  //Serial.println(freq);
/**********************************************************************/
  getMPUData();      // SerialPrint_exp_angle();
  //SerialPrint_q_angle_chart();
  if (radio.available()) {
    getExp();
  }
  diff_angle.y  = exp_angle.pitch - q_angle.pitch;
  diff_angle.x  = exp_angle.roll  - q_angle.roll;
  diff_angle.z  = exp_angle.yaw   - q_angle.yaw;

  // PID Algorithms
  Pitch = UpdatePID( &PID_Motor, ( exp_angle.pitch - q_angle.pitch ), gyro.y );
  Roll  = UpdatePID( &PID_Motor, ( exp_angle.roll  - q_angle.roll  ), gyro.x );
   
  //Yaw   = PID_Yaw.P   * diff_angle.z - PID_Yaw.D   * gyro.z;

  Thr += throttle;
  Thr = constrain( Thr, 1000, 1600 );
  Serial.println(Thr);
  // Output the throttle to motors    + -model
  Motor[0] = (int16_t)(Thr - Pitch        );//+ Yaw);
  Motor[2] = (int16_t)(Thr + Pitch        );//+ Yaw);
  Motor[1] = (int16_t)(Thr         - Roll );//- Yaw);
  Motor[3] = (int16_t)(Thr         + Roll );//- Yaw);
  
  for(byte i = 0; i < 4; i++) {
//    if(Motor[i] < 1000) Motor[i] = 1000;
//    else if(Motor[i] > 2000) Motor[i] = 2000;
    Motor[i] = constrain( Motor[i], 1000, 2000 );
  }

  if(FLY_ENABLE == 0x5A) {
    writeMotor();
  } else {
    writeAllMotor(1000);
  }

  while ((micros() - startLoop) < LOOP_TIME)
  {
    // delay for a accurate cycle time.
    // 100 times per second
  } 
  loopTime =  micros() - startLoop; //Calculating loop_time to calculate frequency
  
 // printMotor();
}



//  Motor[2] = (int16_t)(Thr - Pitch - Rool - Yaw );    //M3
//  Motor[0] = (int16_t)(Thr + Pitch + Rool - Yaw );    //M1
//  Motor[3] = (int16_t)(Thr - Pitch + Rool + Yaw );    //M4
//  Motor[1] = (int16_t)(Thr + Pitch - Rool + Yaw );    //M2


