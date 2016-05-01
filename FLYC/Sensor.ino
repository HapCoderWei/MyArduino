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
      // turn on the DMP, now that it's ready
      //Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
      attachInterrupt(0, dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      //Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      uint8_t FS_SEL = 0;
      uint8_t READ_FS_SEL = mpu.getFullScaleGyroRange();
//      Serial.print("FS_SEL = ");
//      Serial.println(READ_FS_SEL);
      GYRO_FACTOR = 131.0/(FS_SEL + 1);
      // get expected DMP packet size for later comparison
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
  // put your main code here, to run repeatedly:
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
     mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
     // Remove offsets and scale gyro data  
    gyro.x = (gx - base_x_gyro)/GYRO_FACTOR;
    gyro.y = (gy - base_y_gyro)/GYRO_FACTOR;
    gyro.z = (gz - base_z_gyro)/GYRO_FACTOR;
    acc.x = ax; // - base_x_accel;
    acc.y = ay; // - base_y_accel;
    acc.z = az; // - base_z_accel;
  }
//  Serial.print("accel:\t");
//  Serial.print(acc.x);
//  Serial.print("\t");
//  Serial.print(acc.y);
//  Serial.print("\t");
//  Serial.println(acc.z);

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      //Serial.println(F("FIFO overflow!"));

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) 
        fifoCount = mpu.getFIFOCount();

      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//      mpu.dmpGetEuler(ypr, &q);

      q_angle.yaw   = ypr[0] * 180/M_PI - YAW_OFFSET;
      q_angle.pitch = ypr[1] * 180/M_PI - PITCH_OFFSET;
      q_angle.roll  = ypr[2] * 180/M_PI - ROLL_OFFSET;
      
      blinkState = !blinkState;
      digitalWrite(LED_PIN, blinkState);
  }
}
