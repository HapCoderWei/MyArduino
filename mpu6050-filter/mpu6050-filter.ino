#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "KalmanFilter.h"
#include "MsTimer2.h"

MPU6050 mpu; //实例化一个 MPU6050 对象，对象名称为 mpu
KalmanFilter kalmanfilter;
int16_t ax, ay, az, gx, gy, gz;

//********************angle data*********************//
float Q;
float Angle_ax; //由加速度计算的倾斜角度
float Angle_ay;
float K1 = 0.05; // 对加速度计取值的权重
float angle0 = 0.00; //机械平衡角
int slong;
//********************angle data*********************//

//***************Kalman_Filter*********************//
float Q_angle = 0.001, Q_gyro = 0.005; //角度数据置信度,角速度数据置信度
float R_angle = 0.5 , C_0 = 1;
float timeChange = 5; //滤波法采样时间间隔毫秒
float dt = timeChange * 0.001; //注意：dt的取值为滤波器采样时间
//***************Kalman_Filter*********************//

void setup() {
  // put your setup code here, to run once:
  // 加入I2C总线
  Wire.begin();                            //加入 I2C 总线序列
  Serial.begin(57600);                       //开启串口，设置波特率为 115200
  delay(1500);
  mpu.initialize();                       //初始化MPU6050
  delay(2);
  //5ms定时中断设置  使用timer2    注意：使用timer2会对pin3 pin11的PWM输出有影响，因为PWM使用的是定时器控制占空比，所以在使用timer的时候要注意查看对应timer的pin口。
  MsTimer2::set(5, inter);
  MsTimer2::start();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(kalmanfilter.angle);
  Serial.print("\t");
  Serial.print(kalmanfilter.angleWTH);
  Serial.print("\t");
  Serial.println(kalmanfilter.angleGravity);
}

//////////////////////////////////////////////////////////
//////////////////中断定时 5ms定时中断////////////////////
/////////////////////////////////////////////////////////
void inter()
{
  sei();
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);     //IIC获取MPU6050六轴数据 ax ay az gx gy gz
  kalmanfilter.Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);       //获取angle 角度和卡曼滤波
  kalmanfilter.Complementary(ax, ay, az, gy, 0.95);
}
