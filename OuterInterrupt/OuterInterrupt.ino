/*
开关连接实验
 接线方法:
 材料：一个轻触开关、一个10kΩ电阻，一个0.1uF电容（可选）
 连接方法：
 开关接在Arduino D3和+5V之间；
 10kΩ电阻接在Arduino D3和GND之间；
 0.1uF电容接在Arduino D3和+5V之间（可以不用电容）；
 */
 
int Button=9; //连接开关到D3。
int LED=13; //连接LED到13（实际上UNO等版本已经有了）。
boolean onoff=LOW; //设定一个变量用于记录开关状态。

void setup()
{
  pinMode(Button,INPUT); //按钮端子作为输入
  pinMode(LED,OUTPUT); //LED端子作为输出
}
void loop(){
  if(digitalRead(Button)==LOW)  //由于本例检测上升沿触发，所以先检测输入是否低电平，
  {
    delay(10); //然后延时一段时间，
    if(digitalRead(Button)==HIGH) //然后检测是不是电平变为高了。是的话，就是刚好按钮按下了。
    {
      digitalWrite(LED,onoff);  //写入当前LED状态onoff，
      onoff=(!onoff); //然后LED状态反转，以便下一次使用。
      delay(10);  //延时一段时间，防止按钮突然断开再按下。
      while(digitalRead(Button)==HIGH) //判断按钮状态，如果仍然按下的话，等待松开。防止一直按住导致LED输出端连续反转
      {
        delay(1);
      }
    }
  }
}

