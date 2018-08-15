#include <dht.h>

#define dhtPin 2
#define PIN 13

dht DHT;

String data;

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
  } else if(data.equals("led on")) {  // LED ON
    digitalWrite(PIN,HIGH);
  } else if(data.equals("led off")){  // LED OFF
    digitalWrite(PIN,LOW);
  } else if(data.equals("tem&hum")){  // Temp & Humi
    DHT.read11(dhtPin);
    Serial.print(DHT.temperature);
    Serial.println(DHT.humidity);
  } else {
    Serial.println("Wrong Command.");
  }
}
