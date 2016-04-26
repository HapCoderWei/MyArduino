void nrfSet() {
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
}
void getExp() {
  int yaw, pitch, roll;
  
  radio.read(&recBuf, sizeof(recBuf));
  throttle        = recBuf[0];
  yaw   = recBuf[1];
  exp_angle.yaw   = yaw   / 100.0 * Angle_Max;
  pitch = recBuf[2];
  exp_angle.pitch = pitch / 100.0 * Angle_Max;
  roll  = recBuf[3];
  exp_angle.roll  = roll  / 100.0 * Angle_Max;
  FLY_ENABLE = recBuf[4];
  
  // Check for Special byte TxBuf[31]
  //Serial.println(recBuf[31]);
  if(recBuf[4] == 0) {
    //Serial.println(recBuf[4]);
//    writeAllMotor(1100);
//    delay(2300);
    
    while(1) {
      writeAllMotor(1000);
    }
  }
}

