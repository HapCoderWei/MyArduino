#include <PinChangeInt.h>    
//#include <PinChangeIntConfig.h>
#include <TimerOne.h>       


//这两行定义使用PORTD来连接接收机，即Arduino的数字口0-7
#define NO_PORTB_PINCHANGES
#define NO_PORTC_PINCHANGES
#define PIN_COUNT 6    //这里只用五个通道，可以自行增减
#define MAX_PIN_CHANGE_PINS PIN_COUNT


byte pin[] = {2,3,4,5,6,7};    //定义用数字口2-6连接通道1-5
unsigned int time[] = {0,0,0,0,0,0}; // 这里储存遥控信号，数值一般为[1000,2000]左右


byte state=0;
byte i=0;      //用来遍历每个通道
int count = 0; //用来记录loop（）被执行了多少次


void setup() 
{
    Serial.begin(115200);
    Serial.println("PinChangeInt ReciverReading test");


    Timer1.initialize(2200);    //初始化定时器范围为2200ms，因为一般遥控信号不会超过这个值
    Timer1.stop();               
    Timer1.restart();           //把定时器设为0，启动后，定时器会从0加到2200，再从2200减到0，不断往复


    for (byte i=0; i<PIN_COUNT; i++)
    {
        pinMode(pin[i], INPUT);     //把Arduino引脚设为输入
        digitalWrite(pin[i], HIGH); //把Arduino引脚设为内部上拉
    }
    PCintPort::attachInterrupt(pin[i],rise,RISING); // 把PinChangeInt连在第一个通道上，上升沿触发
}


void loop() 
{
  count++;
  //如果已经循环12000次，输出数据，因为数据读取非常快，不必每次读取都输出数据</span>
  if(count == 15000)
  {
        Serial.print("Channel:\t");
        for (byte i=0; i<PIN_COUNT;i++)
        {
            Serial.print(i);
            Serial.print(":");
            Serial.print(time[i]);
            Serial.print(" ");            
        }
        Serial.println();        
        count = 0;
  }
    
    switch (state)
    {
        //若刚刚已经在上升沿触发了中断，把中断改成下降沿触发，以此来计算高电平持续时间，即遥控信号
        case RISING: //we have just seen a rising edge
            PCintPort::detachInterrupt(pin[i]);
            PCintPort::attachInterrupt(pin[i], fall, FALLING); 
            state=255;
            break;
        case FALLING: //we just saw a falling edge
            PCintPort::detachInterrupt(pin[i]);
            i++;                
            i = i % PIN_COUNT;  //i取值在0-PIN_COUNT
            PCintPort::attachInterrupt(pin[i], rise,RISING);
            state=255;
            break;
    }
}


void rise()        //上升沿触发的中断处理函数
{
    Timer1.restart();        //定时器归零
    Timer1.start();            //开始计时
    state=RISING;
}


void fall()        //下降<span style="font-family: Arial, Helvetica, sans-serif;">沿触发的中断处理函数</span>
{
    state=FALLING;
    time[i]=Timer1.read();                                
    Timer1.stop();
}
