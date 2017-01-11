#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

double angleX = 0, degX, dGyX;
double angleY = 0, degY, dGyY;

void setup(){
  Serial.begin(57600);
  mpu6050_init();
}

void loop(){
  mpu6050_start();

  //상보필터 X축 기준
  degX = atan2(AcY, AcZ) * 180 / PI;
  dGyX = GyX / 131; //16bit 250 deg/sec
  angleX = 0.98*(angleX + dGyX*0.001) + 0.02*degX;
  Serial.print(angleX);Serial.print("     ");
  Serial.println(degX);
  
  //상보필터 Y축 기준
//  degY = atan2(AcX, AcZ) * 180 / PI;
//  dGyY = GyY / 131; //16bit 250 deg/sec
//  angleY = 0.98*(angleY + dGyY*0.001) + 0.02*degY;
//  Serial.print(degY);Serial.print("     ");
//  Serial.println(angleY);
  
  delay(3);
}

void mpu6050_init() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void mpu6050_start() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
