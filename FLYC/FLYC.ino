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
#define LED_PIN 13 // (Arduino is 13)
#define g     9.27f

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  mpuSet();
  nrfSet();
  motorSet();
  PIDSet();
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  delay(1500);
}
void loop() {
  getMPUData();
//  Serial.print("ypr\t");
//  Serial.print(q_angle.yaw);
//  Serial.print("\t");
//  Serial.print(q_angle.pitch);
//  Serial.print("\t");
//  Serial.println(q_angle.roll);
  if (radio.available()) {
    getExp();
  }
  diff_angle.y  = exp_angle.pitch - q_angle.pitch;
  diff_angle.x  = exp_angle.roll  - q_angle.roll;
  diff_angle.z  = exp_angle.yaw   - q_angle.yaw;
  // get rid of the gravity component (+1g = +8192 in standard DMP FIFO packet, sensitivity is 2g)
  diff_acc_z  = (acc.z / 8192) * g;   // difference of acclerate in Z

  // PID Algorithms
  Pitch = PID_Motor.P * diff_angle.y - PID_Motor.D * gyro.y;  // PD, don't use I
  Roll  = PID_Motor.P * diff_angle.x - PID_Motor.D * gyro.x;  // PD, don't use I
  Yaw   = PID_Motor.P * diff_angle.z - PID_Motor.D * gyro.z;

  //Thr = 0.001 * throttle * throttle;
  Thr = throttle;
  //Serial.println(diff_acc_z);
  //Thr -= 20 * diff_acc_z;  // negative feedback for Z axis

  //Yaw = -2 * (gyro.z - last_yaw + exp_angle.yaw/1.2); // the -20 should be the PID_Yaw.D
  //last_yaw = q_angle.yaw;
//  // Output the throttle to motors   // X-model
//  Motor[2] = (int16_t)(Thr - Pitch - Rool - Yaw );    //M3
//  Motor[0] = (int16_t)(Thr + Pitch + Rool - Yaw );    //M1
//  Motor[3] = (int16_t)(Thr - Pitch + Rool + Yaw );    //M4
//  Motor[1] = (int16_t)(Thr + Pitch - Rool + Yaw );    //M2

  // +-model
  Motor[0] = Thr + Pitch  ;//      + Yaw;
  Motor[2] = Thr - Pitch   ;//     + Yaw;
  Motor[1] = Thr         + Roll - Yaw;
  Motor[3] = Thr         - Roll - Yaw;
  
  for(int i = 0; i < 4; i++) {
    if(Motor[i] < 1000) Motor[i] = 1000;
    if(Motor[i] > 2000) Motor[i] = 2000;
  }
  writeMotor();
  //printMotor();
}


