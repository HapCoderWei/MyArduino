// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
 
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
 
#include "MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu(0x68);
 
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
 
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
 
 
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
 
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
 
 
 
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
 
//void mpu_setup() {
void setup(){
  Serial.begin(115200);        // opens serial port, sets data rate to 9600 bps
  //Serial.begin(38400);
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin(); 
 
  // initialize device
  Serial.println("Initializing I2C devices...");
  mpu.initialize();
 
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 
  delay(2);
 
  // load and configure the DMP
  Serial.println("Initializing DMP...");
  devStatus = mpu.dmpInitialize();
 
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println("Enabling DMP...");
    mpu.setDMPEnabled(true);
 
    // enable Arduino interrupt detection
    Serial.println("Enabling interrupt detection (Arduino external interrupt 0)...");
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
 
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println("DMP ready! Waiting for first interrupt...");
    dmpReady = true;
 
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
  }
}
 
//void mpu_loop()
void loop()
{
  float alpha,omiga;
 
  // if programming failed, don't try to do anything
  if (!dmpReady)
    return;
 
  // wait for MPU interrupt or extra packet(s) available
  if (!mpuInterrupt && fifoCount < packetSize)
    return;
 
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
 
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
 
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println("FIFO overflow!");
 
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
 
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
 
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
 
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  //从DMP中取出Yaw、Pitch、Roll三个轴的角度，放入数组ypr。单位：弧度
    alpha=-ypr[2] * 180/M_PI;
 
    omiga=mpu.getRotationX()/16.4;        //配置是16位表示正负2000°/s, 65536/4000
 
   /* Serial.print("Yaw: ");
    Serial.print(-ypr[0] * 180/M_PI);
    Serial.print("\tPitch: ");
    Serial.print(-ypr[1] * 180/M_PI);
    Serial.print("\tRoll: ");
    Serial.println(-ypr[2] * 180/M_PI);*/
    Serial.print("q;\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.println(q.z);
    //Serial.print("Alpha ");
    //Serial.print(alpha);//x轴平行偏移角度
    //Serial.print("\tOmiga ");
   // Serial.println(omiga);//对应加速度力道
 
  }
}

