void dmpDataReady() {
    mpuInterrupt = true;
}
void mpuSet() {
  // put your setup code here, to run once:
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  //Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
//  Serial.println(F("Testing device connections..."));
//  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // wait for ready
//  Serial.println(F("\nSend any character to begin DMP programming and demo: "));
//  while (Serial.available() && Serial.read()); // empty buffer
//  while (!Serial.available());                 // wait for data
//  while (Serial.available() && Serial.read()); // empty buffer again

  // load and configure the DMP
//  Serial.println(F("Initializing DMP..."));
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
void getGyro(struct GYRO_Def *gyro, int *data) {
//  gyro->x = abs((float)data[0]) / 16.4;
//  gyro->y = abs((float)data[1]) / 16.4;
//  gyro->z = abs((float)data[2]) / 16.4;
  gyro->x = (float)data[0] / 16.4;
  gyro->y = (float)data[1] / 16.4;
  gyro->z = (float)data[2] / 16.4;
//
//  Serial.print("Gyro\t");
//  Serial.print(data[0]);
//  Serial.print("\t");
//  Serial.print(data[1]);
//  Serial.print("\t");
//  Serial.println(data[2]);
}
void getAccel(struct ACCL_Def *acc, int *data) {
  acc->x = (float)data[0] / 8192.0 * g;
  acc->y = (float)data[1] / 8192.0 * g;
  acc->z = (float)data[2] / 8192.0 * g;
}
void getAngle() {
  int16_t data[3];
  while (fifoCount < packetSize) 
        fifoCount = mpu.getFIFOCount();

      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;

      mpu.dmpGetGyro(data, fifoBuffer);   // get raw gyro value
      getGyro( &gyro, data);   // convert data[3] to gyro[x, y, z]
      mpu.dmpGetAccel(data, fifoBuffer);
      getAccel( &acc, data);
      
      mpu.dmpGetQuaternion(&q, fifoBuffer);
//      mpu.dmpGetGravity(&gravity, &q);
//      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.dmpGetEuler(ypr, &q);
      
      q_angle.yaw   = ypr[0] * 180/M_PI - YAW_OFFSET;
      q_angle.pitch = ypr[1] * 180/M_PI - PITCH_OFFSET;
      q_angle.roll  = ypr[2] * 180/M_PI - ROLL_OFFSET;
}
void getMPUData() {
  // put your main code here, to run repeatedly:
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) ;

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
      getAngle();

      blinkState = !blinkState;
      digitalWrite(LED_PIN, blinkState);
  }
}
