void PIDSet() {
  PID_Motor.P = 0.8; //    1.35 is roll OK
  PID_Motor.I = 0.0;
  PID_Motor.D = 3.2;   //  < 3.0   //0.55 is roll OK
  PID_Motor.dState = 0.0;
  PID_Motor.iState = 0.0;
  PID_Motor.iMax = 50;  // 5 percent is OK
  PID_Motor.iMin = -50;

  PID_Yaw .P = 1.0;
  PID_Yaw .I = 0.0;
  PID_Yaw .D = 0.0;
  PID_Yaw.dState = 0.0;
  PID_Yaw.iState = 0.0;
  PID_Yaw.iMax = 20;
  PID_Yaw.iMin = -20;

  PID_GYRO .P = 2.5;
  PID_GYRO .I = 0.0;
  PID_GYRO .D = 0.0;
  PID_GYRO.dState = 0.0;
  PID_GYRO.iState = 0.0;
  PID_GYRO.iMax = 20;
  PID_GYRO.iMin = -20;
}
float UpdatePID(struct PID *pid, float error, float deravitive) {
  float pTerm, dTerm, iTerm;
  // Calculate the proportional term
  pTerm = pid->P * error;
  // Calculate the integral state with appropriate limiting
  pid->iState += error;
  if(pid->iState > pid->iMax) pid->iState = pid->iMax;
  else if(pid->iState < pid->iMin) pid->iState = pid->iMin;
  // Calculate the integral term
  iTerm = pid->I * pid->iState;
  // Calculate the deravitive term
  dTerm = pid->D * deravitive;

  return pTerm + iTerm - dTerm;
}
float UpdatePID_GYRO(struct PID *pid, float error, float now_position) {
  float pTerm, dTerm, iTerm;
  // Calculate the proportional term
  pTerm = pid->P * error;
  // Calculate the integral state with appropriate limiting
  pid->iState += error;
  if(pid->iState > pid->iMax) pid->iState = pid->iMax;
  else if(pid->iState < pid->iMin) pid->iState = pid->iMin;
  // Calculate the integral term
  iTerm = pid->I * pid->iState;
  // Calculate the deravitive term
  dTerm = pid->D * (now_position - pid->dState);
  pid->dState = now_position;

  return pTerm + iTerm - dTerm;
}

