void PIDSet() {
  PID_Motor.P = 3; //    1.35 is roll OK
  PID_Motor.I = 0.0;
  PID_Motor.D = 50.0;   //  0.55 is roll OK
  PID_Motor.dState = 0.0;
  PID_Motor.iState = 0.0;
  PID_Motor.iMax = 200;
  PID_Motor.iMin = -200;

  PID_Yaw .P = 1.0;
  PID_Yaw .I = 0.0;
  PID_Yaw .D = 0.0;
  PID_Yaw.dState = 0.0;
  PID_Yaw.iState = 0.0;
  PID_Yaw.iMax = 20;
  PID_Yaw.iMin = -20;
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

