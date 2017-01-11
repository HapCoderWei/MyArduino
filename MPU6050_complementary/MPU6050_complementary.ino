#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu;

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// Use the following global variables and access functions to help store the overall
// rotation angle of the sensor
unsigned long last_read_time;
float         last_x_angle;  // These are the filtered angles
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  // Store the gyro angles to compare drift
float         last_gyro_y_angle;
float         last_gyro_z_angle;

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}

inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return last_x_angle;}
inline float get_last_y_angle() {return last_y_angle;}
inline float get_last_z_angle() {return last_z_angle;}
inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}

//  Use the following global variables 
//  to calibrate the gyroscope sensor and accelerometer readings
float    base_x_gyro = 0;
float    base_y_gyro = 0;
float    base_z_gyro = 0;
float    base_x_accel = 0;
float    base_y_accel = 0;
float    base_z_accel = 0;

// This global variable tells us how to scale gyroscope data
float    GYRO_FACTOR;

// This global varible tells how to scale acclerometer data
float    ACCEL_FACTOR;

// Variables to store the values from the sensor readings
int16_t ax, ay, az;
int16_t gx, gy, gz;

unsigned long startLoop = 0;
unsigned long loopTime = 1;
unsigned int freq = 0;

// ================================================================
// ===                CALIBRATION_ROUTINE                       ===
// ================================================================
// Simple calibration - just average first few readings to subtract
// from the later data
void calibrate_sensors() {
  int       num_readings = 10;

  // Discard the first reading (don't know if this is needed or
  // not, however, it won't hurt.)
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Read and average the raw values
  for (int i = 0; i < num_readings; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    base_x_gyro += gx;
    base_y_gyro += gy;
    base_z_gyro += gz;
    base_x_accel += ax;
    base_y_accel += ay;
    base_y_accel += az;
  }
  
  base_x_gyro /= num_readings;
  base_y_gyro /= num_readings;
  base_z_gyro /= num_readings;
  base_x_accel /= num_readings;
  base_y_accel /= num_readings;
  base_z_accel /= num_readings;
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(57600);

  // Set the full scale range of the gyro
  uint8_t FS_SEL = 0;
  //mpu.setFullScaleGyroRange(FS_SEL);

  // get default full scale value of gyro - may have changed from default
  // function call returns values between 0 and 3
  uint8_t READ_FS_SEL = mpu.getFullScaleGyroRange();
  Serial.print("FS_SEL = ");
  Serial.println(READ_FS_SEL);
  GYRO_FACTOR = 131.0/(FS_SEL + 1);
  

  // get default full scale value of accelerometer - may not be default value.  
  // Accelerometer scale factor doesn't reall matter as it divides out
  uint8_t READ_AFS_SEL = mpu.getFullScaleAccelRange();
  Serial.print("AFS_SEL = ");
  Serial.println(READ_AFS_SEL);

  pinMode(LED_PIN, OUTPUT);
  
  // get calibration values for sensors
  calibrate_sensors();
  set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  const float RADIANS_TO_DEGREES = 57.2958; //180/3.14159
  startLoop = micros();
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
              
  // Get time of last raw data read
  unsigned long t_now = millis();
    
  // Remove offsets and scale gyro data  
  float gyro_x = (gx - base_x_gyro)/GYRO_FACTOR;
  float gyro_y = (gy - base_y_gyro)/GYRO_FACTOR;
  float gyro_z = (gz - base_z_gyro)/GYRO_FACTOR;
  float accel_x = ax; // - base_x_accel;
  float accel_y = ay; // - base_y_accel;
  float accel_z = az; // - base_z_accel;
  
        
  float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_z = 0;

  // Compute the (filtered) gyro angles
  float dt =(t_now - get_last_time())/1000.0;
  float gyro_angle_x = gyro_x*dt + get_last_x_angle();
  float gyro_angle_y = gyro_y*dt + get_last_y_angle();
  float gyro_angle_z = gyro_z*dt + get_last_z_angle();
  
  // Compute the drifting gyro angles
  float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
  float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
  float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();     
  
  // Apply the complementary filter to figure out the change in angle - choice of alpha is
  // estimated now.  Alpha depends on the sampling rate...
  const float alpha = 0.96;
  float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
  float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
  float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
  
  // Update the saved data with the latest values
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);

   //Serial.print("CMP:");
   Serial.print(get_last_x_angle(), 2);
   Serial.print(" ");
   Serial.print(get_last_y_angle(), 2);
   Serial.print(" ");
   Serial.println(-get_last_z_angle(), 2);
//   Serial.print("\t");
//   Serial.println(freq);
   
   blinkState = !blinkState;
   digitalWrite(LED_PIN, blinkState);
}
