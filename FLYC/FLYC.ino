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
/*********** end of header ************/
#define LED_PIN 13 // (Arduino is 13)
#define g     9.27f

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  mpuSet();
  nrfSet();
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  getMPUData();
  Serial.print("ypr\t");
  Serial.print(gravity.x);
  Serial.print("\t");
  Serial.print(gravity.y);
  Serial.print("\t");
  Serial.println(gravity.z);
  if (radio.available())
  {
    getExp();
  }
  diff_angle.y  = exp_angle.pitch - q_angle.pitch;
  diff_angle.x  = exp_angle.roll  - q_angle.roll;
  
  diff_acc_z  = gyro.z   - g;   // difference of acclerate in Z

  // PID Algorithms
  Pitch = PID_Motor.P * diff_angle.y - PID_Motor.D * gyro.y;
  Rool  = PID_Motor.P * diff_angle.x - PID_Motor.D * gyro.x;

  Thr = 0.001 * throttle * throttle;
  Thr -= 80 * diff_acc_z;

  Yaw = -20 * (gyro.z + exp_angle.yaw/1.2); // the -20 should be the PID_Yaw.D

  // Output the throttle to motors
  Motor[2] = (int16_t)(Thr - Pitch -Rool- Yaw );    //M3
  Motor[0] = (int16_t)(Thr + Pitch +Rool- Yaw );    //M1
  Motor[3] = (int16_t)(Thr - Pitch +Rool+ Yaw );    //M4
  Motor[1] = (int16_t)(Thr + Pitch -Rool+ Yaw );    //M2

}


