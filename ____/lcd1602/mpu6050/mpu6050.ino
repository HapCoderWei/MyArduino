#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
bool blinkState = false;
 
void setup() {
 
    Wire.begin();
    Serial.begin(38400);
 
    accelgyro.initialize();
 }
 void loop() {
 
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print("a/g:\t");
    Serial.print(ax/16384); Serial.print("\t");
    Serial.print(ay/16384); Serial.print("\t");
    Serial.print(az/16384); Serial.print("\t");
    Serial.print(gx/131); Serial.print("\t");
    Serial.print(gy/131); Serial.print("\t");
    Serial.println(gz/131);
    blinkState = !blinkState;
 
}

