String data;
const int PIN = 13;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available()) ;
  data = Serial.readStringUntil('@');
  
  if(data.equals("Hi")) {
    Serial.println("Hello, I'm Arduino.");
  } else if(data.equals("led on")) {
    digitalWrite(PIN,HIGH);
  } else if(data.equals("led off")){
    digitalWrite(PIN,LOW);
  } else {
    Serial.println("Wrong Command.");
  }
}
