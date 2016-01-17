struct Angle {
  float yaw   = 0;
  float pitch = 0;
  float roll  = 0;
};
struct Angle q_angle;    // real angle read from mpu6050
struct Angle exp_angle;  // expected angle received from Remote Control
VectorFloat diff_angle;

struct PID {
  float P = 1.4;
  float I = 0.0;
  float D = 0.0;
};
PID PID_Motor;

float diff_acc_z, last_yaw = 0.0; // save diff of accl in Z
float throttle;   // save throttle receive from NRF24l01
float Thr = 0, Roll = 0, Pitch = 0, Yaw = 0;


