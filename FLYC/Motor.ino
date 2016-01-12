void motorSet() {
  motor[0].attach(3, 1000, 2000);
  motor[1].attach(5, 1000, 2000);
  motor[2].attach(6, 1000, 2000);
  motor[3].attach(9, 1000, 2000);
}
void writeMotor() {
  motor[0].writeMicroseconds(Motor[0]);
  motor[1].writeMicroseconds(Motor[1]);
  motor[2].writeMicroseconds(Motor[2]);
  motor[3].writeMicroseconds(Motor[3]);
}
void PIDSet() {
  PID_Motor.P = 1.4;
  PID_Motor.I = 0.0;
  PID_Motor.D = 1.1;
}
void printMotor() {
  Serial.print("Motor[0-3]: ");
  Serial.print(Motor[0]);
  Serial.print("\t");
  Serial.print(Motor[1]);
  Serial.print("\t");
  Serial.print(Motor[2]);
  Serial.print("\t");
  Serial.println(Motor[3]);
  //Serial.println(throttle);
}

