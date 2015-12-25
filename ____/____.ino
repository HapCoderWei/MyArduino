void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}
void loop()
{
  int RXData;
  
  if(Serial.available())
  {
    RXData = Serial.read();
    Serial.println(RXData);
    if(RXData == '1')
    {
      digitalWrite(13,HIGH);
      delay(100);
    }
    if(RXData == '4')
    {
      digitalWrite(13,HIGH);
      delay(500);
    }
    if(RXData == '7')
    {
      digitalWrite(13,HIGH);
      delay(1000);
    }
    else digitalWrite(13,LOW);
  }
}
