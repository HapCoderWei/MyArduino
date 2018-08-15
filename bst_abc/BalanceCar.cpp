/*
BalanceCar.h BalanceCar.cpp - Library for BST-Balance car code.
Created by SKY ZHU&ROOMS LUO, OCTOBER 2, 2016.
JUST FOR THE Company of Technology of yahboom.
In order to  avoid Infringement Act,this core is not for the commerce except being authorized by the writer.
*/

#include "./BalanceCar.h"

//////////////////速度PI//////////////////
double BalanceCar::speedpiout(double kps,double kis,double kds,int f,int b,double p0)
{
  float speeds = (pulseleft + pulseright) * 1.0;                                                                 //车速脉冲值
  pulseright = pulseleft = 0;
  speeds_filterold *= 0.7;                                                                     //一阶互补滤波
  float speeds_filter = speeds_filterold + speeds * 0.3;
  speeds_filterold = speeds_filter;
  positions += speeds_filter;
  positions += f;                                                                         //全进控制量融合
  positions += b;                                                                         //全进控制量融合
  positions = constrain(positions, -3000,3000); //抗饱和积分
  double output = kis * (p0 - positions) + kps * (p0 - speeds_filter);                      //速度环控制PI
  if(flag1==1)
  {
  positions=0;
  }
  
  return output;
}
//////////////////�ٶ�PI////////////////////

float BalanceCar::turnspin(int turnleftflag,int turnrightflag,int spinleftflag,int spinrightflag,double kpturn,double kdturn,float Gyroz)
{
  int spinonce = 0;
  float turnspeed = 0;
	float rotationratio = 0;
   float turnout_put = 0;
	
  if (turnleftflag == 1 || turnrightflag == 1 || spinleftflag == 1 || spinrightflag == 1)
  {
    if (spinonce == 0)                             //��תǰ�жϵ�ǰ���٣���ǿС����Ӧ�ԡ�
    {
      turnspeed = ( pulseright + pulseleft);                      //С����ǰ���� �����ʾ
	  spinonce++;
    }

    if (turnspeed < 0)                                 //С����ǰ�ٶȾ���ֵ
    {
      turnspeed = -turnspeed;

    }
    if(turnleftflag==1||turnrightflag==1)
    {
     turnmax=3;
     turnmin=-3;
    }
    if(spinleftflag==1||spinrightflag==1)
    {
      turnmax=10;
      turnmin=-10;
    }
    rotationratio = 5 / turnspeed;                        //����С���ٶ��趨ֵ
    if (rotationratio < 0.5)rotationratio = 0.5;
    if (rotationratio > 5)rotationratio = 5;
  }
  else
  {
    rotationratio = 0.5;
    spinonce = 0;
    turnspeed = 0;
  }
  if (turnleftflag == 1 || spinleftflag == 1)//���ݷ����������
  {
    turnout += rotationratio;
  }
  else if (turnrightflag == 1 || spinrightflag == 1)//���ݷ����������
  {
    turnout -= rotationratio;
  }
  else turnout = 0;
  if (turnout > turnmax) turnout = turnmax;//��ֵ���ֵ����
  if (turnout < turnmin) turnout = turnmin;//��ֵ��Сֵ����

  turnout_put = -turnout * kpturn - Gyroz * kdturn;//��תPD�㷨���� �ں��ٶȺ�Z����ת��λ��
	return turnout_put;
}

void BalanceCar::pwma(double speedoutput,float rotationoutput,float angle,float angle6,int turnleftflag,int turnrightflag,int spinleftflag,int spinrightflag,
	int f,int b,float accelz,int Pin1,int Pin2,int Pin3,int Pin4,int PinPWMA,int PinPWMB)
{

  pwm1 = -angleoutput - speedoutput - rotationoutput; //Left电机PWM输出值
  pwm2 = -angleoutput - speedoutput + rotationoutput;//Right电机PWM输出值

  //幅度值限制
  if (pwm1 > 255) pwm1 = 255;
  if (pwm1 < -255) pwm1 = -255;
  if (pwm2 > 255) pwm2 = 255;
  if (pwm2 < -255) pwm2 = -255;


  //角度过大,停止电机
  if (angle > 30 || angle < -30)
  {
    pwm1 = 0;
    pwm2 = 0;
  }
  //

 


		  if (angle6 > 10 || angle6 < -10 &turnleftflag == 0 & turnrightflag == 0 & spinleftflag == 0 & spinrightflag == 0 && f == 0 && b == 0)
  {
           if(stopl + stopr > 1500||stopl + stopr < -3500)
   {
    pwm1 = 0;
    pwm2 = 0;
	flag1=1;

	}
  }
  else 
  {
   stopl=stopr=0;
   flag1=0;

  }



//电机的正负输出判断   左电机判断
if (pwm1 >= 0) {
  digitalWrite(Pin2, 0);
  digitalWrite(Pin1, 1);
  analogWrite(PinPWMA, pwm1);
} else {
  digitalWrite(Pin2, 1);
  digitalWrite(Pin1, 0);
  analogWrite(PinPWMA, -pwm1);
}
//电机的正负输出判断   右电机判断
if (pwm2 >= 0) {
  digitalWrite(Pin4, 0);
  digitalWrite(Pin3, 1);
  analogWrite(PinPWMB, pwm2);
} else {
  digitalWrite(Pin4, 1);
  digitalWrite(Pin3, 0);
  analogWrite(PinPWMB, -pwm2);
}

}
