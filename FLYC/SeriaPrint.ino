void SerialPrint_q_angle() {
  Serial.print("ypr\t");
  Serial.print(q_angle.yaw);
  Serial.print("\t");
  Serial.print(q_angle.pitch);
  Serial.print("\t");
  Serial.println(q_angle.roll);
}
void SerialPrint_exp_angle() {
  Serial.print("exp_angle\t");
  Serial.print(exp_angle.yaw);
  Serial.print("\t");
  Serial.print(exp_angle.pitch);
  Serial.print("\t");
  Serial.println(exp_angle.roll);
}
void SerialPrint_PitchRollYaw() {
  Serial.print("Pitch: ");
  Serial.print(Pitch);
  Serial.print("\tRoll: ");
  Serial.print(Roll);
  Serial.print("\tYaw: ");
  Serial.println(Yaw);
}
void SerialPrint_Gyro() {
  Serial.print("gyro.x: ");
  Serial.print(gyro.x);
  Serial.print("\tgyro.y: ");
  Serial.print(gyro.y);
  Serial.print("\tgyro.z: ");
  Serial.println(gyro.z);
}
void SerialPrint_Pitch_GyroY() {
  Serial.print("q_angle.pitch: ");
  Serial.print(q_angle.pitch);
  Serial.print("\tgyro.y: ");
  Serial.println(gyro.y);
}

