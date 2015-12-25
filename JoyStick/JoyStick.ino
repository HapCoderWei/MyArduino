int val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("S-Y: ");
  val = analogRead(5);
  Serial.println(val);
}
