void dmpDataReady() {
    mpuInterrupt = true;
}
// ================================================================
// ===                CALIBRATION_ROUTINE                       ===
// ================================================================
// Simple calibration - just average first few readings to subtract
// from the later data
void calibrate_sensors() {
  int  num_readings = 10;
  
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

  Serial.print("base_x_gyro: ");
  Serial.print(base_x_gyro);
  Serial.print("\t ");
  Serial.print(base_y_gyro);
  Serial.print("\t ");
  Serial.println(base_z_gyro);
}
void mpuSet() {
  // put your setup code here, to run once:
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  //Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(-1);
  mpu.setYGyroOffset(-31);
  mpu.setZGyroOffset(15);
  mpu.setZAccelOffset(1534); // I have changed this

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
      mpu.setDMPEnabled(true);
      attachInterrupt(0, dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();
      dmpReady = true;
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
//      Serial.print(F("DMP Initialization failed (code "));
//      Serial.print(devStatus);
//      Serial.println(F(")"));
  }
}
void getMPUData() {
  int16_t data[3];
  // put your main code here, to run repeatedly:
  if (!dmpReady) return;

  while (!mpuInterrupt && fifoCount < packetSize) {
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      mpu.resetFIFO();
  } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) 
        fifoCount = mpu.getFIFOCount();

      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      getYawPitchRoll(&q, ypr);
//      mpu.dmpGetGravity(&gravity, &q);
//      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.dmpGetGyro(data, fifoBuffer);  // data[0] = x, data[1] = y, data[2] = z
      gyro.x = data[0] / 16.4;
      gyro.y = data[1] / 16.4;
      gyro.z = data[2] / 16.4;

      q_angle.yaw   = ypr[0] * 180/M_PI - YAW_OFFSET;
      q_angle.pitch = ypr[1] * 180/M_PI - PITCH_OFFSET;
      q_angle.roll  = ypr[2] * 180/M_PI - ROLL_OFFSET;
  }
}
void getYawPitchRoll(Quaternion *q, float *ypr) 
{
  ypr[0] = atan2(2.0*(q->w*q->z + q->x*q->y), 1 - 2.0*(q->y*q->y + q->z*q->z));
  ypr[1] = safe_asin(2.0*(q->w*q->y - q->x*q->z));
  ypr[2] = atan2(2.0*(q->w*q->x + q->y*q->z), 1 - 2.0*(q->x*q->x + q->y*q->y));
}
float safe_asin(float v) 
{
  if(isnan(v)) {
    return 0.0;
  }
  if(v >= 1.0) {
    return M_PI / 2;
  }
  if(v <= -1.0) {
    return -M_PI / 2;
  }
  return asin(v);
}

