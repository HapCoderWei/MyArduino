// 光驱步进电机进角为18°
// 电机每走一步转动18度，则转一圈需要360/18*（每步需要时间）
void setup() {                
  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}


void loop() {
  digitalWrite(9, HIGH);
  delay(3);          
  digitalWrite(9, LOW); 
  delay(3);          
}
