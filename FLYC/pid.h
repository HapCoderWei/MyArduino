struct Angle {
  float yaw   = 0;
  float pitch = 0;
  float roll  = 0;
};
struct Angle q_angle;    // real angle read from mpu6050
struct Angle exp_angle;  // expected angle received from Remote Control
VectorFloat diff_angle;

struct PID {
  float P;
  float I;
  float D;
};
PID PID_Motor;

float diff_acc_z; // save diff of accl in Z
float throttle;   // save throttle receive from NRF24l01
float Thr = 0, Rool = 0, Pitch = 0, Yaw = 0;
float Motor[4];

