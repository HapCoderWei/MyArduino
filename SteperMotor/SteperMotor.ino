/*
 * 步进电机跟随电位器旋转
 * (或者其他传感器)使用0号模拟口输入
 * 使用arduino IDE自带的Stepper.h库文件
 */
 
#include <Stepper.h>
 
// 这里设置步进电机旋转一圈是多少步
#define STEPS 100
 
// attached to设置步进电机的步数和引脚
Stepper stepper(STEPS, 8, 9, 10, 11);
 
// 定义变量用来存储历史读数
int previous = 0;
 
void setup()
{
  // 设置电机每分钟的转速为90步
  stepper.setSpeed(100);
}
 
void loop()
{
  // 获取传感器读数
  int val = analogRead(0);
 
  // 移动步数为当前读数减去历史读数
  stepper.step(val - previous);
  //stepper.step(1);
 
  // 保存历史读数
  previous = val;
}

