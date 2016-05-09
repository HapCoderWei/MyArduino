MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
//struct GYRO_Def {
//  float x = 0.0;
//  float y = 0.0;
//  float z = 0.0;
//} gyro;       // [x, y, z]            gyro sensor measurements
//struct ACCL_Def {
//  float x = 0.0;
//  float y = 0.0;
//  float z = 0.0;
//} acc;        // [x, y, z]            acc  sensor measurements
VectorFloat gyro, acc;
int16_t ax, ay, az;
int16_t gx, gy, gz;
float    base_x_gyro = 0;
float    base_y_gyro = 0;
float    base_z_gyro = 0;
float    base_x_accel = 0;
float    base_y_accel = 0;
float    base_z_accel = 0;
float GYRO_FACTOR;
float ACCEL_FACTOR;
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

const float YAW_OFFSET   = 0.22;
const float PITCH_OFFSET = -2.81;
const float ROLL_OFFSET  = -0.17;

